#include "minishell.h"

// cc readline.c parser.c table.c hash.c load_envv.c pipes.c -L libft -lreadline -lft

int	main(int argc, char **argv, char **env)
{
	char			*input;
	char			*expin;
	struct sigaction sa;
	t_data			data;
	//t_test_env		testvars;

	// interesting, arrays can be initialised like this
	int	arr[3] = {-1};

	//data.vars = &testvars;
	test_envvars(&data);

//	return (0);
	// * set the memory allocated for sigaction struct to 0
	memset(&sa, 0, sizeof(sa));
	// sets function call for signal 
	sa.sa_handler = handle_sigint;
	// is setting the set of signals to 0, however this is redundatnt since
	// memset achieves the same thing since the entire memory area of the struct is set to 0
	sigemptyset(&sa.sa_mask);
	// this is also redundant, but various flags can be set, and may need to be
	sa.sa_flags = 0;

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		return (1);
	}

	//readintobuff(2);
	chain_pipes(4);

	return(0);
	//load_envv();

	rl_initialize();
	rl_readline_name = "minishell";
	printf("unbash \\o_o/ \n");

		input = readline(">>>");
		if (input)
		{
			add_history(input);
			//expin = expand_env(input, &data);
			//free(input);
			tokenise(input, &data);
			printf("########################\n");
			data.tok = ft_split(input, -1);
			print_tokens(data.tok);
			// rl_replace_line("all chinese history is good and the government is perfect", 1);
			// rl_redisplay();
			// sleep(1);
			// printf("Input added to history.\n");
			//free(expin);
			int i = 0;
			while (data.tok[i])
			{
				if (is_var(&data, data.tok[i], '$'))
				{
					expand_envv(&data, data.tok[i]);
					// free(data.tok[i]);
					// data.tok[i] = data.expand;
				}
				i++;
			}
			printf("#########################\n");
			print_tokens(data.tok);
		}
	append_history(3, "history");
}

int	test_envvars(t_data *data)
{
	init_arr(data, VAR_BUFF);
	data->uev[poly_r_hash("one", VAR_BUFF)] = create_node("one", "unruffled");
	data->uev[poly_r_hash("two", VAR_BUFF)] = create_node("two", "sleep");
	data->uev[poly_r_hash("three", VAR_BUFF)] = create_node("three", "melatonine");
	
	printf("%s\n", data->uev[poly_r_hash("two", VAR_BUFF)]->key);
	printf("%s\n", data->uev[poly_r_hash("two", VAR_BUFF)]->var);

	return (0);
}

t_args	*init_test()
{
	int		i;
	t_args	*args;

	args = malloc(3 * sizeof(t_args));
	
	wrt_to_str("/usr/bin/ls", &args[0].cmd);
	args[0].arg = malloc(3 * sizeof(char *));
	wrt_to_str("ls", &args[0].arg[0]);
	wrt_to_str("-l", &args[0].arg[1]);
	args[0].arg[2] = NULL;

	wrt_to_str("/usr/bin/sort", &args[1].cmd);
	args[1].arg = malloc(3 * sizeof(char *));
	wrt_to_str("sort", &args[1].arg[0]);
	wrt_to_str("-r", &args[1].arg[1]);
	args[1].arg[2] = NULL;
	
	wrt_to_str("/usr/bin/grep", &args[2].cmd);
	args[2].arg = malloc(3 * sizeof(char *));
	wrt_to_str("grep", &args[2].arg[0]);
	wrt_to_str("h", &args[2].arg[1]);
	args[2].arg[2] = NULL;

	i = 0;
    while (i < 3)
	{
        printf("cmd: %s\n", args[i].cmd);
        printf("args:");
        for (int j = 0; args[i].arg[j] != NULL; j++)
            printf(" %s", args[i].arg[j]);
        printf("\n");
		i++;
    }
	return (args);
}

int chain_pipes(int n)
{
    int		i; 
	int		j;
    t_pp 	pp;
    t_args	*args;

    args = init_test();
    
    pp.buff = malloc(n * sizeof(char *));
    pp.ends = malloc(((n - 1) * 2) * sizeof(int));
    pp.pid = malloc(n * sizeof(pid_t));

	i = 0;
	while (i < n - 1)
	{
		pipe(pp.ends + i * 2);
		i++;
	}

    i = 0;
	while (i < n)
	{
        pp.pid[i] = fork();
        if (pp.pid[i] == 0)
		{
            if (i != 0)
                dup2(pp.ends[(i - 1) * 2], STDIN_FILENO);
            if (i != n - 1)
                dup2(pp.ends[(i * 2) + 1], STDOUT_FILENO);
			j = 0;
			while (j < (n - 1) * 2)
			{
                close(pp.ends[j]);
				j++;
            }
            execve(args[i].cmd, args[i].arg, NULL);
            exit(1);
        }
		i++;
    }

	i = 0;
	while (i < (n - 1) * 2)
	{
    	if (i != (n - 2) * 2)
        	close(pp.ends[i]);
    	i++;
	}

    pp.buff[0] = malloc(BUFF_SZ * sizeof(char));
    pp.bytes_r = read(pp.ends[(n - 2) * 2], pp.buff[0], BUFF_SZ - 1);
    pp.buff[0][pp.bytes_r] = '\0';

    close(pp.ends[(n - 2) * 2]);

	i = 0;
	while (i < n)
	{		
        wait(NULL);
		i++;
    }

    printf("buffer: \n%s\n", pp.buff[0]);

    free(pp.buff[0]);
    free(pp.buff);
    free(pp.ends);
    free(pp.pid);

    return 0;
}
