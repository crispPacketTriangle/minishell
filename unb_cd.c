#include "minishell.h"

// to determine if a file is symbolic link
// stat
// lstat
// fstat
int	unb_cd(t_args *args, t_data *data)
{
	char	*cwd;
	char	*path;

	path = args->arg[1];
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
	(void)path;
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
