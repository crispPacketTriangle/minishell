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

void	handle_sigint(int sig);
int		tokenise(char *input);
void	print_tokens(char **line);

#endif
