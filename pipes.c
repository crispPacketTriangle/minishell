#include "minishell.h"

// order of execution
// lex and parse input
// pass variable substitution argument to subshell
// save output to a buffer
// when running main pipeline, substitute buffer
// so we will leave the last process as directing
// output to stdout and redirect the output of the
// process in main to buffer
// the last pipe must be defined and passed to chain_pipes
//   along with n (number of pipies) and possilby other
//   set-up variables that the parser defines
int chain_pipes(t_data *data)
{
	int		i; 
	int		j;
	int		n;
	t_pp	pp;
	t_args	**args;
	int		status;
	char	**paths;

	status = 0;
	//args = init_test();
	args = data->p_cmd_set;
	
	// here set the full paths to commands
	paths = get_paths(data);
	//path = get_path(fpath, paths, X_OK);
	//if (NULL == path)
	//	retval = ENOENT;


	n = n_process(data);

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
			args[i]->cmd = get_path((const char *)args[i]->arg[0], paths, X_OK);
			// check if command is builtin
			execve(args[i]->cmd, args[i]->arg, NULL);
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
	// pp.buff[0] = malloc(BUFF_SZ * sizeof(char));
	// pp.bytes_r = read(pp.ends[(n - 2) * 2], pp.buff[0], BUFF_SZ - 1);
	// pp.buff[0][pp.bytes_r] = '\0';
	close(pp.ends[(n - 2) * 2]);
	i = 0;
	while (i < n)
	{	
		//wait(NULL);
		waitpid(pp.pid[i], &status, 0);
		status = (status >> 8) & 0xFF;
		i++;
	}
	// printf("buffer: \n%s", pp.buff[0]);
	// free(pp.buff[0]);
	// free(pp.buff);
	//status = (status >> 8) & 0xFF;
	ft_printf("status: %d\n", status);
	free(pp.ends);
	free(pp.pid);
	return 0;
}

int	n_process(t_data *data)
{
	int	n;

	n = 0;
	while (data->redir)
	{
		if (data->redir->dir[0] == '|')
			n++;
		data->redir = data->redir->next;
	}
	return (n + 1);
}

