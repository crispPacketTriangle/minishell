#ifndef MINISHELL_H
#define MINISHELL_H

# define _XOPEN_SOURCE 700
# define EXIT_STATUS 1

# ifndef VAR_BUFF
	# define VAR_BUFF 100
# endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/time.h>
#include <errno.h>
#include <limits.h>
#include "libft/libft.h"

extern char **environ;

# define BUFF_SZ 4096

// pipe chain variables
typedef struct	t_pp
{
	char	**buff;
	ssize_t	bytes_r;
	pid_t	*pid;
	int		*ends;
}	t_pp;

// process variables
typedef struct	t_args
{
	char	*cmd;
	char	**arg;
}	t_args;

// key value dictionary entry
typedef struct	t_key_val
{
	char			*key;
	char			*val;
}	t_key_val;

// user defined environment variables
// key value dictionary entry as node
// in linked list for separate chaning
// hashing implementation
typedef struct	t_var_tb
{
	char			*key;
	char			*var;
	struct t_var_tb	*next;
}	t_var_tb;
//
// typedef struct	t_test_env
// {
// 	t_var_tb	*head;
// 	t_var_tb	*node1;
// 	char		*key1;
// 	char		*key2;
// 	char		*var1;
// 	char		*var2;
// }	t_test_env;

// mega structure
typedef struct	t_data
{
	int			tog;    	// toggle opening or closing qt marks (*= -1) 
	int			nqts;   	// number of qt marks
	int			sqts;  		// numbrt of single qt marks
	int			dqts;   	// number of double qt marks
	char		lst;    	// previous character
	int			paren;		// parens opened closed
	int			np;     	// number of pipes
	int			lvars;   	// length of variables
	int			lvals;		// length of values
	int			key_iter;	// iterator for temp env var keys char **keys
	char		*pdir;		// previous directory
	t_key_val	**envv;		// environment variables key value pairs
	t_key_val	**keys;
	t_args		*args;		// cmd arg groupings in order of execution
	char		**tok;
	char		*expand;
	t_var_tb	**uev;		// user defined variables key value pairs
}	t_data;

void		handle_sigint(int sig);
void		init_data(t_data *data);
int			tokenise(char *input, t_data *data);
int			m_set(char c, char *set);
void		print_tokens(char **line);
void		init_tog(char *input, t_data *data);
void		init_qts(t_data *data, int p);
void		init_len(t_data *data);
int			tog_on(t_data *data);
void		parens(char c, t_data *data);
void		mid(char c, t_data *data);
int			exit_status(char c);
int			count_dollars(char *input, t_data *data);
char		*expand_env(char *input, t_data *data);
void		store_key(int step, int i, char *input, t_data *data);
void		print_keys(t_data *data);
int			load_envv(void);

t_var_tb	*create_node(char *key, char *var);
int			add_var(t_var_tb **head, t_var_tb *node);

int			iter_table(t_var_tb *node, void (f)(void *,
							t_data *, char *), t_data *data, char *key);
void		print_node(void *node, t_data *data);
void		free_table(t_var_tb **node);
int			wrt_to_str(char *src, char **dst);
void		cal_lvals(void *d, char *key, t_data *data);
char		*expand(char *input, t_data *data);

int			init_arr(t_data *data, int n);
int			poly_r_hash(char *key, int n);
int			append_envv(t_data *data);
int			is_var(t_data *data, char *str, char c);
void		expand_var(void *d, t_data *data, char *key);
int			expand_envv(t_data *data, char *str);

int			readintobuff(int n);
int			chain_pipes(int n);

int			unb_cd(t_args *args, t_data *data);
char		*home_dir(char *path);
char		*prev_dir(char *cwd, char *path, t_data *data);
char		*mod_path(char *cwd, char *path, t_data *data);
char		*set_pdir(char *cwd, t_data *data);
int 		free_cd(char *cwd);
char		*subpath(char **path, char *cpypath, char *env);

void		unb_echo(t_args *args);
int			option(char *str, int *n);
int			len_arr(char **arr);

int			errsub(int macro);
int			perrsub();

int			dev_placeholders(char *input, t_data *data);
void		test_cd_cmd_args(t_data *data);
void		test_echo_cmd_args(t_data *data);
void		init_cd_test(t_data *data);
void		init_echo_test(t_data *data);
void		test_echo_cmd_args(t_data *data);
int			test_envvars(t_data *data);
t_args		*init_test();

#endif
