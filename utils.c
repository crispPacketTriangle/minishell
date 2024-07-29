#include "minishell.h"

int	dev_placeholders(char *input, t_data *data)
{
	if (ft_strncmp(input, "pipes", 5) == 0)
		chain_pipes(data);
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
			//ft_printf("%d: ", i);
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
	if (ft_strncmp(input, "clear", 5) == 0)
	{
		clear_terminal();
		//system("clear");
		return(1);
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

void	clear_terminal(void)
{
	char	*clear_term;
	char	*reset_buff;

	clear_term = "\033[H\033[2J";
	reset_buff = "\033[3J";
	write(STDOUT_FILENO, clear_term, 7);
	write(STDOUT_FILENO, reset_buff, 4);
}


void		handle_sigint(int sig)
{
	ft_printf("handle singnal %d\n");
}

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
	wrt_to_str("", &args[2].arg[1]);
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
	data->args[0].arg[0] = data->tok[0]; 
	data->args[0].arg[1] = data->tok[1]; 
}

void	init_cd_test(t_data *data)
{
	data->args = malloc(sizeof(t_args));
	data->args[0].arg = malloc(2 * sizeof(char *));
}


