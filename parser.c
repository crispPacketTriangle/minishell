#include "minishell.h"

// how are input strings from readline formatted? is everything taken as literals? so " "" " is " \" \" "

// cat input.txt | tr ' ' '\n' | sort | uniq -c | sort -nr > word_count.txt
// cat access.log | awk '{print $1}' | sort | uniq -c | sort -nr | head -n 10 > top_ips.txt
// grep -Eo '[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,6}' emails.txt | sort | uniq > unique_emails.txt
// top -b -n 1 | grep "Cpu(s)" | awk '{print $2 + $4 "% CPU"}' > cpu_usage.txt
// cat /var/log/syslog /var/log/auth.log | grep -i "error" | sort | uniq > combined_errors.txt

void	handle_sigint(int sig)
{
	printf("sig: %d\n", sig);
}

// should not count as evironment variable if enclosed by single qt mks
// so should have a state machine for single qt mks
int	expand_env(char *input, t_data *data)
{
	int		i;
	int		step;

	init_qts(data);
	// first count n $ in str
	data->keys = malloc ((count_dollars(input, data) + 1) * sizeof(char *));
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			mid(input[i], data);
		if (input[i] == '$' && data->sqts == 0)
		{
			if (exit_status(input[i + 1]))
			{
				i++;
				continue;
			}
			step = 0;
			i++;
			while (ft_isalnum(input[i]))
			{
				i++;
				step++;
			}
			store_key(step, i, input, data);
		}
		i++;
	}
	data->keys[data->key_iter] = NULL;
	print_keys(data);
	return (0);
}

void	store_key(int step, int i, char *input, t_data *data)
{
	int	j;

	data->keys[data->key_iter] = malloc((step + 1) * sizeof(char));
	i -= step;
	j = 0;
	while (j < step)
	{
		data->keys[data->key_iter][j] = input[i];
		i++;
		j++;
	}
	data->keys[data->key_iter][j] = '\0';
	data->key_iter++;
}

void	print_keys(t_data *data)
{
	int	i;

	i = 0;
	while (data->keys[i])
	{
		printf("key: %s\n", data->keys[i]);
		i++;
	}
}

int	count_dollars(char *input, t_data *data)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	init_qts(data);
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			mid(input[i], data);
		if (input[i] == '$' && input[i + 1] != '?' && data->sqts == 0)
			n++;
		i++;
	}
	printf("n: %d\n", n);
	return (n);
}

int	return_exit_status()
{
	return (0);
}

int	exit_status(char c)
{
	return(0);
}

int	tokenise(char *input, t_data *data)
{
	int	i;

	i = 0;
	init_qts(data);
	while (input[i])
	{
		if (input[i] == '|')
			data->np++;
		if (input[i] == '\'' || input[i] == '\"')
			mid(input[i], data);
		if (data->nqts == 0)
		{
			if (input[i] == ' ')
				input[i] = -1;
		}
		i++;
	}
	return (0);
}

void	mid(char c, t_data *data)
{
	if (c == data->lst)
		data->tog *= - 1;
	data->lst = c;
	if (c == '\'')
		data->sqts += data->tog;
	if (c == '\"')
		data->dqts += data->tog;
	data->nqts += data->tog;
}

void	init_qts(t_data *data)
{
	data->tog = 1;
	data->nqts = 0;
	data->sqts = 0;
	data->dqts = 0;
	data->lst = 'c';
	data->key_iter = 0; 
}

void	print_tokens(char **line)
{
	int	i;

	i = 0;
	while(line[i])
	{
		printf("%s\n", line[i]);
		i++;
	}
}

// void	init_tog(char *input, t_data *data)
// {
// 	data->ns = 0;
// 	data->nd = 0;
// }

