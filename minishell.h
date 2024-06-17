#ifndef MINISHELL_H
#define MINISHELL_H

# define _XOPEN_SOURCE 700
# define EXIT_STATUS 1

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

typedef struct t_args
{
	char	*cmd;
	char	**arg;
}	t_args;

typedef struct t_data
{
	int		tog;    // toggle opening or closing qt marks (*= -1) 
					// 		added to nqts for each occurence of qt marks
	int		nqts;   // number of qt marks
	int		sqts;   // numbrt of single qt marks
	int		dqts;   // number of double qt marks
	char	lst;    // previous character
	// int		ns;
	// int		nd;
	int		np;     // number of pipes
	int		key_iter;
	char	**keys; //
	t_args	**args;
}	t_data;

void	handle_sigint(int sig);
int		tokenise(char *input, t_data *data);
void	print_tokens(char **line);
void	init_tog(char *input, t_data *data);
void	init_qts(t_data *data);
int		tog_on(t_data *data);
void	mid(char c, t_data *data);
int		exit_status(char c);
int		count_dollars(char *input, t_data *data);
int		expand_env(char *input, t_data *data);
void	store_key(int step, int i, char *input, t_data *data);
void	print_keys(t_data *data);

#endif
