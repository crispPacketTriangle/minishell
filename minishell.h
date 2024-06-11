#ifndef MINISHELL_H
#define MINISHELL_H

# define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/time.h>
#include "libft/libft.h"

extern char **environ;

typedef struct t_data
{
	int	*tog;
	int	n;
}	t_data;

void	handle_sigint(int sig);
int		tokenise(char *input, t_data *data);
void	print_tokens(char **line);
void	init_tog(char *input, t_data *data);
int		tog_on(t_data *data);

#endif
