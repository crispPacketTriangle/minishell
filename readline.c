#include "minishell.h"

// cc readline.c parser.c table.c hash.c load_envv.c pipes.c utils.c unb_vars.c -L libft -lreadline -lft

int	main(int argc, char **argv, char **env)
{
	char				*input;
	struct sigaction	sa;
	t_data				data;
	int					i;

	init_data(&data);
	test_envvars(&data);
	init_en_var_table(env, &data);

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
			i = 0;
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

int	unb_pwd(void)
{
	char	*cwd;

	cwd = malloc(PATH_MAX * sizeof(char));
	if (!cwd)
		return (perrsub());
	getcwd(cwd, PATH_MAX);
	ft_printf("%s\n", cwd);	
	free(cwd);
	return (0);
}
