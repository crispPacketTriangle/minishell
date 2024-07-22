#include "minishell.h"

void	unb_echo(t_args *args)
{
	int	i;
	int	n;
	int	strlen;
	
	n = 1;
	i = 1;
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
	data->args[0].arg = malloc((n + 1) * sizeof(char *));
}

void	test_echo_cmd_args(t_data *data)
{
	int	i;

	data->args[0].cmd = data->tok[0];
	i = 0;
	while(data->tok[i])
	{
		data->args[0].arg[i] = data->tok[i];
		i++;
	}
	data->args[0].arg[i] = NULL;
}
