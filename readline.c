#include "minishell.h"

// cc readline.c parser.c table.c hash.c load_envv.c pipes.c utils.c -L libft -lreadline -lft

int	main(int argc, char **argv, char **env)
{
	char				*input;
	struct sigaction	sa;
	t_data				data;

	init_data(&data);

	test_envvars(&data);

	// * set the memory allocated for sigaction struct to 0
	ft_memset(&sa, 0, sizeof(sa));
	// sets function call for signal 
	sa.sa_handler = handle_sigint;
	// is setting the set of signals to 0, however this is redundatnt since
	// memset achieves the same thing since the entire memory area of the struct is set to 0
	sigemptyset(&sa.sa_mask);
	// this is also redundant, but various flags can be set, and may need to be
	sa.sa_flags = 0;

	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (1);
	
	// while (*environ)
	// {
	// 	printf("e: %s\n", *environ);
	// 	environ++;
	// }

	rl_initialize();
	rl_readline_name = "minishell";
	printf("unbash \\o_o/ \n");

	// have getcwd() run before the loop
	// set the prompt to result and then
	// set the result each time directory
	// is changed
	while (1)
	{
		input = readline(">>>");
		if (input)
		{
			add_history(input);
			tokenise(input, &data);
			data.tok = ft_split(input, -1);
			if (dev_placeholders(input, &data) != 0)
				continue ;
			int i = 0;
			while (data.tok[i])
			{
				if (is_var(&data, data.tok[i], '$'))
					expand_envv(&data, data.tok[i]);
				i++;
			}
			print_tokens(data.tok);
			free(data.tok);
		}
		free(input);
		append_history(3, "history");
	}
}

void	unb_pwd()
{
	
}

// all builtins should, even if not required by the subject
// be implemented in a way that makes them expandable with
// different flags. Perhaps with function pointers. In bash
// flags can be groupd together -ne, meaning the builtin
// functions should handle this. 

void	unb_echo(t_args *args)
{
	int	i;
	int	n;
	int	strlen;
	
	n = 1;
	i = 0;
	while (option(args->arg[i], &n))
		i++;
	while (args->arg[i])
	{
		strlen = ft_strlen(args->arg[i]);
		write(1, args->arg[i], strlen);
		i++;
		if (args->arg[i])
			write(1, " ", 1);
	}
	if (n == 1)
		write(1, "\n", 1);
}

int	option(char *str, int *n)
{
	if (str && ft_strncmp(str, "-n", 3) == 0)
	{
		*n = 0;
		return (1); 
	}
	return (0);
}

int	len_arr(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

void	init_echo_test(t_data *data)
{
	int	n;

	n = len_arr(data->tok);
	data->args = malloc(sizeof(t_args));
	data->args[0].arg = malloc(n * sizeof(char *));
}

void	test_echo_cmd_args(t_data *data)
{
	int	i;

	data->args[0].cmd = data->tok[0];
	i = 1;
	while(data->tok[i])
	{
		data->args[0].arg[i - 1] = data->tok[i];
		i++;
	}
	data->args[0].arg[i - 1] = NULL;
}
