#include "minishell.h"

void	handle_sigint(int sig)
{
	printf("sig: %d\n", sig);
}

// a key_val struct is created for each $var in the input string
// the keys are added and then used to look up and add the value from
// the  user defined env var dictionary (or if not found the system env
// vars.  If the env var does not exist an empty string is added to the 
// value
char	*expand_env(char *input, t_data *data)
{
	int		i;
	int		step;

	init_qts(data);
	data->keys = malloc ((count_dollars(input, data) + 1) * sizeof(t_key_val *));
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
	append_envv(data);
	//iter_table(data->vars->head, &cal_lvals, data); // all that needs to change is this function
													// and the init table function for testing keys
	return(expand(input, data));
}

// create new string with expanded $vars and return it
char	*expand(char *input, t_data *data)
{
	int		i;
	int		j;
	int		t;
	char	*tmpstr;
	int		len;

	i = 0;
	t = 0;
	data->key_iter = 0;
	data->lst = 'c';
	data->nqts = 0;
	data->tog = 1;
	data->sqts = 0;
	data->dqts = 0;
	len = (ft_strlen(input) - data->lvars) + data->lvals;
	tmpstr = malloc((len + 1) * sizeof(char));
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			mid(input[i], data);
		if (input[i] == '$' && data->sqts == 0)
		{
			i++;
			while (input[i] && ft_isalnum(input[i]))
				i++;
			if (data->keys[data->key_iter]->val)
			{
				j = 0;
				while(data->keys[data->key_iter]->val[j])
				{
					tmpstr[t] = data->keys[data->key_iter]->val[j];
					t++;
					j++;
				}
			}
			data->key_iter++;
		}
		if (input[i])
		{
			tmpstr[t] = input[i];
			i++;
			t++;
		}
	}
	tmpstr[t] = '\0';
	// printf("temp: %s\n", tmpstr);
	return (tmpstr);
}

// parses out environment variable names as keys
void	store_key(int step, int i, char *input, t_data *data)
{
	int	j;
	
	data->keys[data->key_iter] = malloc(sizeof(t_key_val));
	data->keys[data->key_iter]->key = malloc((step + 1) * sizeof(char));
	data->keys[data->key_iter]->val = NULL;
	i -= step;
	j = 0;
	while (j < step)
	{
		data->keys[data->key_iter]->key[j] = input[i];
		i++;
		j++;
	}
	data->keys[data->key_iter]->key[j] = '\0';
	data->lvars += (step + 1);
	data->key_iter++;
}

int	append_envv(t_data *data)
{
	int	i;
	int idx;

	i = 0;
	while (data->keys[i])
	{
		idx = poly_r_hash(data->keys[i]->key, VAR_BUFF);
		if (data->uev[idx])
		{
			iter_table(data->uev[idx], &cal_lvals, data, data->keys[i]);
		}
		i++;
	}
	return (0);
}

// calculate length of values and sum them to kv->var
// also add the values to the keys parsed from the input string
void	cal_lvals(void *d, t_key_val *keys, t_data *data)
{
	t_var_tb	*kv; 
	kv = (t_var_tb *)d;
	if (ft_strncmp(keys->key, kv->key, ft_strlen(keys->key) + 1) == 0)
	{
		data->lvals += ft_strlen(kv->var);
		keys->val = kv->var;
	}
}


void	print_keys(t_data *data)
{
	int	i;

	i = 0;
	while (data->keys[i])
	{
		printf("key: %s\n", data->keys[i]->key);
		i++;
	}
}

// count environment variables (ignore $? and $ inside single qts)
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
	// printf("n: %d\n", n);
	return (n);
}

int	return_exit_status()
{
	return (0);
}

// not implemented. For branching, or dealing with scenarios where
// $ does not denote env var
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

// check if inside qts. update single and double qts variables
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
	data->lvars = 0;
	data->lvals = 0;
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

