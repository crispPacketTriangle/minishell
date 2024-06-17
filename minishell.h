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

typedef struct	t_args
{
	char	*cmd;
	char	**arg;
}	t_args;

typedef struct	t_var_tb
{
	char			*key;
	char			*var;
	struct t_var_tb	*next;
}	t_var_tb;

typedef struct	t_key_val
{
	char			*key;
	char			*val;
}	t_key_val;

typedef struct	t_test_env
{
	t_var_tb	*head;
	t_var_tb	*node1;
	char		*key1;
	char		*key2;
	char		*var1;
	char		*var2;
}	t_test_env;

typedef struct	t_data
{
	int			tog;    	// toggle opening or closing qt marks (*= -1) 
	int			nqts;   	// number of qt marks
	int			sqts;  		// numbrt of single qt marks
	int			dqts;   	// number of double qt marks
	char		lst;    	// previous character
	int			np;     	// number of pipes
	int			lvars;   	// length of variables
	int			lvals;		// length of values
	int			key_iter;	// iterator for temp env var keys char **keys 
	t_key_val	**keys;		// temp environment variable keys
	t_args		**args;
	t_test_env	*vars;
}	t_data;

void		handle_sigint(int sig);
int			tokenise(char *input, t_data *data);
void		print_tokens(char **line);
void		init_tog(char *input, t_data *data);
void		init_qts(t_data *data);
int			tog_on(t_data *data);
void		mid(char c, t_data *data);
int			exit_status(char c);
int			count_dollars(char *input, t_data *data);
char		*expand_env(char *input, t_data *data);
void		store_key(int step, int i, char *input, t_data *data);
void		print_keys(t_data *data);


t_var_tb	*create_node(char *key, char *var);
int			add_var(t_var_tb **head, t_var_tb *node);
int			iter_table(t_var_tb *node, void (f)(void *, t_data *), t_data *data);
void		print_node(void *node, t_data *data);
void		free_table(t_var_tb **node);
void		wrt_to_str(char *src, char *dst);
int			test_envvars(t_data *data);
void		cal_lvals(void *d, t_data *data);
char		*expand(char *input, t_data *data);

#endif
