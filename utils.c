#include "minishell.h"


// todo: implement functions for user variable insertion into
// dictionary. if conflict at index append to list
int	test_envvars(t_data *data)
{
	init_arr(data);
	data->uev[poly_r_hash("one")] = create_node("one", "unruffled");
	data->uev[poly_r_hash("two")] = create_node("two", "sleep");
	data->uev[poly_r_hash("three")] = create_node("three", "melatonine");
	
	// printf("%s\n", data->uev[poly_r_hash("two", VAR_BUFF)]->key);
	// printf("%s\n", data->uev[poly_r_hash("two", VAR_BUFF)]->var);

	return (0);
}

t_args	*init_test()
{
	int		i;
	int		j;
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
	wrt_to_str("e", &args[2].arg[1]);
	args[2].arg[2] = NULL;

	// i = 0;
	// while (i < 3)
	// {
	// 	printf("cmd: %s\n", args[i].cmd);
	// 	printf("args:");
	// 	j = 0;
	// 	while (args[i].arg[j] != NULL)
	// 	{
	// 		printf(" %s", args[i].arg[j]);
	// 		j++;
	// 	}
	// 	printf("\n");
	// 	i++;
	// }
	return (args);
}

// in terms of order of execution
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
int chain_pipes(int n)
{
	int		i; 
	int		j;
	t_pp	pp;
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
	// pp.buff[0] = malloc(BUFF_SZ * sizeof(char));
	// pp.bytes_r = read(pp.ends[(n - 2) * 2], pp.buff[0], BUFF_SZ - 1);
	// pp.buff[0][pp.bytes_r] = '\0';
	close(pp.ends[(n - 2) * 2]);
	i = 0;
	while (i < n)
	{	
		wait(NULL);
		i++;
	}
	// printf("buffer: \n%s", pp.buff[0]);
	// free(pp.buff[0]);
	// free(pp.buff);
	free(pp.ends);
	free(pp.pid);
	return 0;
}

// if .
// if ..
// if -
// if /path
// if path
// the chdir function works with both an absolute and a relative path
// if implementing .. or - it would require keeping a library of visited paths
// in the first instance and etiting the current path to remove one level
// in the second
// 
// cd . seems redundant but it has the effect of setting the last directory
// to the current which might have some utility
//
// to determine if a file is symbolic link
// stat
// lstat
// fstat
// should use errno
//
// keep previous directory in memory
// when moving to parent directory edit cwd
// going back a directoy with - prints directoy to console
// 
// chdir doesn't know how to interpret ~/ so should be
// expanded to the full directory and then involves looking up env
// vars as the current user should be appended to /home/<user>
// HOME=/home/cpt
int	unb_cd(t_args *args, t_data *data)
{
	char	*cwd;
	char	*path;

	path = args->arg[0];
	cwd = malloc(PATH_MAX * sizeof(char));
	if (!cwd)
		return (perrsub());
	if (getcwd(cwd, PATH_MAX) == NULL)
		return (free_cd(cwd));
	path = mod_path(cwd, path, data);
	if (!path)
		return (free_cd(cwd));
	if (chdir(path) != 0)
		return (free_cd(cwd));
	set_pdir(cwd, data);
	free(cwd);
	return (0);
}

int free_cd(char *cwd)
{
	printf("error\n");
	free(cwd);
	return (perrsub());
}

char	*home_dir(char *path)
{
	char	*cpypath;
	int		size;
	int		i;

	if (!path)
	{
		path = malloc(3 * sizeof(char));
		ft_strlcpy(path, "~/", 3);
	}
	size = ft_strlen(path);
	cpypath = malloc((size + 1) * sizeof(char));
	ft_strlcpy(cpypath, path, size + 1);
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], "HOME=", 5) == 0)
		{
			subpath(&path, cpypath, environ[i]);
			break ;
		}
		i++;
	}
	free(cpypath);
	return (path);
}

char	*subpath(char **path, char *cpypath, char *env)
{
	int		j;
	int		k;
	
	free(*path);
	*path = malloc(PATH_MAX * sizeof(char));
	j = 5;
	k = 0;
	while (env[j])
	{
		(*path)[k] = env[j];
		j++;
		k++;
	}
	j = 1;
	while (cpypath[j])
	{
		(*path)[k] = cpypath[j];
		j++;
		k++;
	}
	(*path)[k] = '\0';
	return (*path);
}

char	*prev_dir(char *cwd, char *path, t_data *data)
{
	if (!data->pdir)
	{
		data->pdir = malloc(PATH_MAX * sizeof(char));
		if (!data->pdir)
			return (NULL);
		ft_strlcpy(data->pdir, cwd, ft_strlen(cwd) + 1);
	}
	return (data->pdir);
}

char	*mod_path(char *cwd, char *path, t_data *data)
{
	if (!path)
		return(home_dir(path));
	if (ft_strncmp(path, "~/", 2) == 0)
		return (home_dir(path));
	if (ft_strncmp(path, "-", 1) == 0)
		return (prev_dir(cwd, path, data));
	return (path);
}

char	*set_pdir(char *cwd, t_data *data)
{	
	if (!data->pdir)
	{
		data->pdir = malloc(PATH_MAX * sizeof(char));
		if (!data->pdir)
			return (NULL);
		ft_strlcpy(data->pdir, cwd, ft_strlen(cwd) + 1);
	}
	ft_memset(data->pdir, 0, PATH_MAX);
	ft_strlcpy(data->pdir, cwd, ft_strlen(cwd) + 1);
	return (data->pdir);
}

// functions to handle errors that we define or else standard errors
// must write the correct error to stderr and return the error value
// from the process
int	errsub(int macro)
{
	return(macro);
}

int	perrsub()
{
	return (errno);
}

// eventually all parsed input will be in the form of an array
// of t_args in the order of executions
void	test_cd_cmd_args(t_data *data)
{
	data->args[0].cmd = data->tok[0];
	data->args[0].arg[0] = data->tok[1]; 
}

void	init_cd_test(t_data *data)
{
	data->args = malloc(sizeof(t_args));
	data->args[0].arg = malloc(sizeof(char *));
}

int	dev_placeholders(char *input, t_data *data)
{
	if (ft_strncmp(input, "pipes", 5) == 0)
		chain_pipes(3);
	if (ft_strncmp(input, "exit", 4) == 0)
		exit(1);
	if (ft_strncmp(input, "pwd", 3) == 0)
	{	
		unb_pwd();
		return (1);
	}
	if (ft_strncmp(input, "cd", 2) == 0)
	{
		init_cd_test(data);
		test_cd_cmd_args(data);
		unb_cd(data->args, data);  // the cmd args struct should be passed to cd not the path directly
										 // cd must be run in the parent shell process
		return (1);
	}
	if (ft_strncmp(input, "echo", 4) == 0)
	{
		init_echo_test(data);
		test_echo_cmd_args(data);
		unb_echo(data->args);
		return (1);
	}
	if (ft_strncmp(input, "env", 3) == 0)
	{
		int	i = 0;
		while (i < VAR_BUFF)
		{
			ft_printf("%d: ", i);
			print_table(data->ent[i]);
			i++;
		}
		return (1);
	}
	if (ft_strncmp(input, "export", 6) == 0)
	{
		t_args	*test;
		test = init_export_test(input, data);
		unb_export(test, data);
		return (1);
	}
	if (ft_strncmp(input, "unset", 5) == 0)
	{
		t_args	*test;
		test = init_export_test(input, data);
		unb_unset(test, data);
		return (1);
	}
	return (0);
}

void	init_data(t_data *data)
{
	data->tog = 0;
	data->nqts = 0;
	data->sqts = 0;
	data->dqts = 0;
	data->lst = 0;
	data->paren = 0;
	data->np = 0;
	data->lvars = 0;
	data->lvals = 0;
	data->key_iter = 0;
	data->pdir = NULL;
	data->envv = NULL;
	data->keys = NULL;
	data->args = NULL;
	data->tok = NULL;
	data->expand = NULL;
	data->uev = NULL;
}
