/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathstrings.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stobin <stobin@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 21:56:59 by stobin            #+#    #+#             */
/*   Updated: 2024/04/19 22:20:55 by stobin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_strarr(char **strarr)
{
	int	i;

	i = 0;
	if (strarr)
	{
		if (strarr[i])
		{
			while (strarr[i] != NULL)
			{
				free(strarr[i]);
				strarr[i++] = NULL;
			}
			free(strarr[i]);
			strarr[i] = NULL;
		}
	}
	free(strarr);
	strarr = NULL;
}

char	*make_path(char *path, char *str)
{
	char	*path0;
	char	*fullpath;

	path0 = ft_strjoin(path, "/");
	fullpath = ft_strjoin(path0, str);
	free(path0);
	return (fullpath);
}

char	**find_paths(char **envp)
{
	int	i;

	i = 0;
	while (!ft_strnstr(envp[i], "PATH=", 5))
		i++;
	return (ft_split(envp[i] + 5, ':'));
}

char	*get_path(const char *str, char **paths, int mode)
{
	int		i;
	int		no_perm;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = make_path(paths[i++], (char *)str);
		no_perm = access(path, F_OK);
		if (!no_perm)
			no_perm = access(path, mode);
		if (!no_perm)
		{
			free_strarr(paths);
			return (path);
		}
		free(path);
	}
	free_strarr(paths);
	return (NULL);
}
