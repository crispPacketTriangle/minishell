/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stobin <stobin@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 12:17:37 by stobin            #+#    #+#             */
/*   Updated: 2024/07/29 21:41:09 by lworden          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	main(int argc, char *argv[])
// {
// 	int			retval;
// 	const char	*fpath;
// 	FILE		*fp;

// 	retval = 0;
// 	if (argc == 1)
// 	{
// 		retval = tokenise(char *input, t_data *data)
// 	}
// 	else
// 	{
// 		fpath = (const char *)argv[1];
// 		// if (!file_exists((const char *)argv[1]))
// 		if (!access(fpath, F_OK))
// 			retval = ENOENT; // maybe handle error more verbosely
// 		else if (!access(fpath, R_OK))
// 			retval = EACCES;
// 		// next search all directories in $PATH
// 		// while (searching)
// 		// {
// 		// 	if (!file_found)
// 		// 		retval = ENOENT;
// 		// }
// 		if (retval)
// 			return (retval);
// 		fp = fopen(fpath, 'r');
// 		if (NULL == fp)
// 			return (errno);
// 		// read and execute contents of file
// 		return (fclose(fp));
// 	}
// }

int	run_interactive_shell(t_data *data)
{
	char	*input;
	int		i;

	// rl_initialize();
	// rl_readline_name = "minishell";
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
			tokenise(input, data);
			data->tok = ft_split(input, -1);
			if (dev_placeholders(input, data) != 0)
				continue ;
			i = 0;
			while (data->tok[i])
			{
				if (is_var(data, data->tok[i], '$'))
					expand_envv(data, data->tok[i]);
				i++;
			}
			print_tokens(data->tok);
			ft_printf("Return value of parse_tokens(): %d\n", parse_tokens(data->tok));
			// continue program logic here (LW)
			free(data->tok);
		}
		free(input);
		append_history(3, "history");
	}
	return (0);
}

char	**get_paths(t_data *data)
{
	int			h_idx;
	t_var_tb	*node;
	char 		**paths;

	paths = NULL;
	h_idx = poly_r_hash("PATH");
	node = data->ent[h_idx];
	while (node && ft_strncmp((const char *)"PATH", (const char *)node->key, 4))
		node = node->next;
	if (NULL != node)
		paths = ft_split(node->var, ':');
	return (paths);
}

void	handle_lines(FILE *fp, t_data *data)
{
	char 	*line;
	size_t	len;
	int		readed;

	line = NULL;
	// read and execute contents of file
	// N.B. MUST REPLACE getline() with get_next_line()!!!
	while ((readed = getline(&line, &len, fp)) != -1)
	{
		tokenise(line, data);
		data->tok = ft_split(line, -1);
		if (dev_placeholders(line, data) != 0)
			continue;
		int i = 0;
		while (data->tok[i])
		{
			if (is_var(data, data->tok[i], '$'))
				expand_envv(data, data->tok[i]);
			i++;
		}
		print_tokens(data->tok);
		ft_printf("Return value of parse_tokens(): %d\n", parse_tokens(data->tok));
		// continue program logic here (LW)
		free(data->tok);
	}
	free(line);
}

int	run_batch_shell(t_data *data, const char *fpath)
{
	char	**paths;
	char	*path;
	FILE	*fp;
	int		tmp;
	int		retval;

	path = NULL;
	retval = 0;
	if (!access(fpath, F_OK))
		retval = ENOENT; // maybe handle error more verbosely
	else if (!access(fpath, R_OK))
		retval = EACCES;
	// next search all directories in $PATH
	paths = NULL;
	if (!ft_strchr(fpath, (int)'/'))
	{
		paths = get_paths(data);
		path = get_path(fpath, paths, R_OK);
		if (NULL == path)
			retval = ENOENT;
	}
	if (!retval)
	{
		fp = fopen(fpath, "r");
		if (NULL == fp)
			retval = errno;
		if (!retval)
			handle_lines(fp, data);
		tmp = fclose(fp);
		if (tmp)
			retval = tmp;
	}
	free (path);
	return (retval);
}

//int	main(int argc, char **argv, char **envp)
//{
//	t_data				data;
//	struct sigaction	sa;
//	int					retval;
//
//	init_data(&data);
//	test_envvars(&data);
//	init_en_var_table(envp, &data);
//
//	// * set the memory allocated for sigaction struct to 0
//	ft_memset(&sa, 0, sizeof(sa));
//	// sets function call for signal 
//	sa.sa_handler = handle_sigint;
//	// is setting the set of signals to 0, however this is redundatnt since
//	// memset achieves the same thing since the entire memory area of the struct is set to 0
//	sigemptyset(&sa.sa_mask);
//	// this is also redundant, but various flags can be set, and may need to be
//	sa.sa_flags = 0;
//
//	if (sigaction(SIGINT, &sa, NULL) == -1)
//		return (1);
//	
//	if (argc == 1)
//		retval = run_interactive_shell(&data);
//	else
//		retval = run_batch_shell(&data, (const char *)argv[1]);
//	return (retval);
//}

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
