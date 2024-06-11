#include "minishell.h"

// cc readline.c parser.c -L libft -lreadline -lft

int	main()
{
	char			*input;
	struct sigaction sa;
	t_data			data;

	// * set the memory allocated for sigaction struct to 0
	memset(&sa, 0, sizeof(sa));
	// sets function call for signal 
	sa.sa_handler = handle_sigint;
	// is setting the set of signals to 0, however this is redundatnt since
	// memset achieves the same thing since the entire memory area of the struct is set to 0
	sigemptyset(&sa.sa_mask);
	// this is also redundant, but various flags can be set, and may need to be
	sa.sa_flags = 0;

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		return (1);
	}

	char **env = environ;
    while (*env) 
	{
        printf("%s\n", *env);
        env++;
    }

	char exmp[] = "cat input.txt | tr ' ' '\n' | sort | uniq -c | sort -nr > word_count.txt";
	
	char exmp2[] = "cat access.log | awk '{print $1}' | sort | uniq -c | sort -nr | head -n 10 > top_ips.txt";
	char exmp3[] = "grep -Eo '[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,6}' emails.txt | sort | uniq > unique_emails.txt";
	char exmp4[] = "top -b -n 1 | grep \"Cpu(s)\" | awk '{print $2 + $4 \"% CPU\"}' > cpu_usage.txt";
	char exmp5[] = "cat /var/log/syslog /var/log/auth.log | grep -i \"error\" | sort | uniq > combined_errors.txt";
	tokenise(exmp, &data);
	tokenise(exmp2, &data);
	tokenise(exmp3, &data);
	tokenise(exmp4, &data);
	tokenise(exmp5, &data);
	// print_tokens(ft_split(exmp, -1));
	// print_tokens(ft_split(exmp2, -1));
	// print_tokens(ft_split(exmp3, -1));
	// print_tokens(ft_split(exmp4, -1));
	// print_tokens(ft_split(exmp5, -1));

	// while (1)
	// {
	// 	printf("zzz\n");
	// 	sleep(1);
	// }

	rl_initialize();
	rl_readline_name = "minishell";
	printf("mini shell activated \\o_o/ \n");

		input = readline(">>>");
		if (input)
		{
			tokenise(input, &data);
			print_tokens(ft_split(input, -1));
			// rl_replace_line("all chinese history is good and the government is perfect", 1);
			// rl_redisplay();
			// sleep(1);
			// printf("Input added to history.\n");
			add_history(input);
			free(input);
		}
	append_history(3, "history");
}

