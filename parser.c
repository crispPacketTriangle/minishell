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

void	expand_env(char *input, t_data *data)
{
	int		i;
	int		step;
	char	key;

	// first count n $ in str
	i = 0;
	step = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			while (ft_isalnum(input[i]))
			{
				i++;
				step++;
			}

		}
	}
}

int	tokenise(char *input, t_data *data)
{
	int	i;

	i = 0;
	data->tog = 1;
	data->nqts = 0;
	data->lst = 'c';
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
	data->nqts += data->tog;
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

void	init_tog(char *input, t_data *data)
{
	data->ns = 0;
	data->nd = 0;
}

