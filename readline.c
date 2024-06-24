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



	// if (pipe(pp.ends) == -1)
	// 	return (1);
	// pp.pid1 = fork();
	// if (pp.pid1 == 0)
	// {
	// 	close(pp.ends[0]);
	// 	if (dup2(pp.ends[1], STDOUT_FILENO) == -1)
	// 		exit(1);
	// 
	// close(pp.ends[1]);
	//
	// char	*args[1];
	// args[0] = "ls";
	// //execve("usr/bin/ls", args, env);
	// execlp("ls", "ls", NULL);
	// }
	// else
	// {
	// 	close(pp.ends[1]);
	// 	pp.bytes_r = read(pp.ends[0], pp.buff, BUFF_SZ -1);
	// 	pp.buff[pp.bytes_r] = '\0';
	// 	close(pp.ends[0]);
	// 	wait(NULL);
	// 	printf("buffer: \n%s\n", pp.buff);
	// }
	readintobuff();

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

int	readintobuff()
{	
	t_pp			pp;

	if (pipe(pp.ends) == -1)
		return (1);
	pp.pid1 = fork();
	if (pp.pid1 == 0)
	{
		close(pp.ends[0]);
		if (dup2(pp.ends[1], STDOUT_FILENO) == -1)
			exit(1);
	
	close(pp.ends[1]);

	char	*args[1];
	args[0] = "ls";
	//execve("usr/bin/ls", args, env);
	execlp("ls", "ls", NULL);
	}
	else
	{
		close(pp.ends[1]);
		pp.bytes_r = read(pp.ends[0], pp.buff, BUFF_SZ -1);
		pp.buff[pp.bytes_r] = '\0';
		close(pp.ends[0]);
		wait(NULL);
		printf("buffer: \n%s\n", pp.buff);
	}
	return (0);
}
