#include "minishell.h"

void	handle_sigint(int sig)
{
	printf("sig: %d\n", sig);
}

int	tokenise(char *input)
{
	int	i;
	int	s_tog;
	int	d_tog;

	i = 0;
	s_tog = 1;
	d_tog = 1;
	while (input[i])
	{
		if (input[i] == '\'')
			s_tog *= -1;
		if (input[i] == '\"')
			d_tog *= -1;
		if (!(s_tog == -1 || d_tog == -1))
		{
			if (input[i] == ' ')
				input[i] = -1;
		}
		i++;
	}
	return (0);
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
	printf("#######################################\n");
}

// how are input strings from readline formatted? is everything taken as literals? so " "" " is " \" \" "

// cat input.txt | tr ' ' '\n' | sort | uniq -c | sort -nr > word_count.txt
// cat access.log | awk '{print $1}' | sort | uniq -c | sort -nr | head -n 10 > top_ips.txt
// grep -Eo '[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,6}' emails.txt | sort | uniq > unique_emails.txt
// top -b -n 1 | grep "Cpu(s)" | awk '{print $2 + $4 "% CPU"}' > cpu_usage.txt
// cat /var/log/syslog /var/log/auth.log | grep -i "error" | sort | uniq > combined_errors.txt

// copy string replacing spaces not between quotation mrks with #
