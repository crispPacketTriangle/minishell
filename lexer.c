#include "minishell.h"

// a non existing variable should return an empty string not nothing at
// all

int	tokenise(char *input, t_data *data)
{
	int	i;

	i = 0;
	init_qts(data, 1);
	while (input[i])
	{
		if (input[i] == '|')
			data->np++;
		if (m_set(input[i], "\'\""))
			mid(input[i], data);
		if (m_set(input[i], "()"))
			parens(input[i], data);
		if (data->nqts == 0 && data->paren == 0)
		{
			if (input[i] == ' ')
				input[i] = -1;
		}
		i++;
	}
	return (0);
}

// check if inside qts. update single and double qts variables
// need another variable to store qt type

// if single or double encountered set toggle, only unset when
// same qt is encountered again

// check if toggle has been set yet
//	if not set toggle on
//	if set toggle off if match
//	else
//		continue
void	mid(char c, t_data *data)
{
	if (data->lst == 'c')
	{
		if (c == '\'')
			data->sqts = 1;
		else
			data->dqts = 1;
		data->nqts = 1;
		data->lst = c;
	}
	else if (c == data->lst)
	{
		if (c == '\'')
			data->sqts = 0;
		else
			data->dqts = 0;
		data->nqts = 0;
		data->lst = 'c';
	}
//	if (c == data->lst)
//		data->tog *= -1;
//	data->lst = c;
//	if (c == '\'')
//		data->sqts += data->tog;
//	if (c == '\"')
//		data->dqts += data->tog;
//	data->nqts += data->tog;
}

void	parens(char c, t_data *data)
{
	if (c == '(')
		data->paren++;
	if (c == ')')
		data->paren--;
}

void	init_qts(t_data *data, int p)
{
	data->tog = 1;
	data->nqts = 0;
	data->sqts = 0;
	data->dqts = 0;
	data->paren = 0;
	data->lst = 'c';
	if (p == 0)
		init_len(data);
	data->key_iter = 0; 
}

void	init_len(t_data *data)
{
	data->lvars = 0;
	data->lvals = 0;
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

int	m_set(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

// do any of the args contain env vars
// do these vars need to be expanded?
//
// do not expand if env var
// 		within single qt mrks
//		within ()	
//
// expand if
// 		whole token env var
// 		within double qt mrks
//

// the token is the lookup key
// if not within qt mks take pointer to token make it point instead 
// to the env var val
// if within " and not ' and not () write to new str free old token 
// and point to new str
// if within $() then is a matter of process order
int	expand_envv(t_data *data, char *str)
{
	int		idx;

	if (!is_var(data, str, '$'))
		return (0);
	if (str[0] == '$')
	{
		str++;
		idx = poly_r_hash(str);
		if (data->uev[idx])
			iter_table(data->uev[idx], &expand_var, data, str);
		// input should be a pointer in the data struct
	}
	// should handle $ in string but within dqts
	else if (str[0] == '\"')
		data->expand = "todo";
	return (0);
	// also handle non existing variables which should return empty str
	// but if non existing in shell vars check 
}

int	is_var(t_data *data, char *str, char c)
{
	int	i;

	if (m_set(str[0], "\'("))
		return (0);
	init_qts(data, 0);
	i = 0;
	while (str[i])
	{
		if (m_set(str[i], "\'\""))
			mid(str[i], data);
		if (m_set(str[i], "()"))
			parens(str[i], data);
		if (str[i] == c && data->sqts == 0 && data->paren == 0)
			return (1);
		i++;
	}
	return (0);
}


// calculate length of values and sum them to kv->var
// also add the values to the keys parsed from the input string
void	expand_var(void *d, t_data *data, char *str)
{
	t_var_tb	*kv;
	int			len;

	(void)*data;
	kv = (t_var_tb *)d;
	if (ft_strncmp(str, kv->key, ft_strlen(str) + 1) == 0)
	{
		str--;
		free(str);
		len = ft_strlen(kv->var);
		str = malloc((len + 1) * sizeof(char));
		ft_strlcpy(str, kv->var, len + 1);
		return ;
	}
	// returning the pointer in every case (could this have led to
	// the garbage values?)
	str--;
}





// void	init_tog(char *input, t_data *data)
// {
// 	data->ns = 0;
// 	data->nd = 0;
// }


// a key_val struct is created for each $var in the input string
// the keys are added and then used to look up and add the value from
// the  user defined env var dictionary (or if not found the system env
// vars.  If the env var does not exist an empty string is added to the 
// value
// char	*expand_env(char *input, t_data *data)
// {
// 	int		i;
// 	int		step;
//
// 	init_qts(data, 0);
// 	data->keys = malloc ((count_dollars(input, data) + 1) * sizeof(t_key_val *));
// 	i = 0;
// 	while (input[i])
// 	{
// 		if (m_set(input[i], "\'\""))
// 			mid(input[i], data);
// 		if (input[i] == '$' && data->sqts == 0)
// 		{
// 			if (exit_status(input[i + 1]))
// 			{
// 				i++;
// 				continue;
// 			}
// 			step = 0;
// 			i++;
// 			while (ft_isalnum(input[i]))
// 			{
// 				i++;
// 				step++;
// 			}
// 			store_key(step, i, input, data);
// 		}
// 		i++;
// 	}
// 	data->keys[data->key_iter] = NULL;
// 	//append_envv(data);
// 	//return(expand(input, data));
// 	return (input);
// }

// int	return_exit_status()
// {
// 	return (0);
// }
//
// // not implemented. For branching, or dealing with scenarios where
// // $ does not denote env var
// int	exit_status(char c)
// {
// 	return(0);
// }

// create new string with expanded $vars and return it
// char	*expand(char *input, t_data *data)
// {
// 	int		i;
// 	int		j;
// 	int		t;
// 	char	*tmpstr;
// 	int		len;
//
// 	i = 0;
// 	t = 0;
// 	init_qts(data, 1);
// 	len = (ft_strlen(input) - data->lvars) + data->lvals;
// 	tmpstr = malloc((len + 1) * sizeof(char));
// 	while (input[i])
// 	{
// 		if (m_set(input[i], "\'\""))
// 			mid(input[i], data);
// 		if (input[i] == '$' && data->sqts == 0)
// 		{
// 			i++;
// 			while (input[i] && ft_isalnum(input[i]))
// 				i++;
// 			if (data->keys[data->key_iter]->val)
// 			{
// 				j = 0;
// 				while(data->keys[data->key_iter]->val[j])
// 				{
// 					tmpstr[t] = data->keys[data->key_iter]->val[j];
// 					t++;
// 					j++;
// 				}
// 			}
// 			data->key_iter++;
// 		}
// 		if (input[i])
// 		{
// 			tmpstr[t] = input[i];
// 			i++;
// 			t++;
// 		}
// 	}
// 	tmpstr[t] = '\0';
// 	return (tmpstr);
// }


// count environment variables (ignore $? and $ inside single qts)
// int	count_dollars(char *input, t_data *data)
// {
// 	int	i;
// 	int	n;
//
// 	i = 0;
// 	n = 0;
// 	init_qts(data, 1);
// 	while (input[i])
// 	{
// 		if (m_set(input[i], "\'\""))
// 			mid(input[i], data);
// 		if (input[i] == '$' && input[i + 1] != '?' && data->sqts == 0)
// 			n++;
// 		i++;
// 	}
// 	// printf("n: %d\n", n);
// 	return (n);
// }


// int	append_envv(t_data *data)
// {
// 	int	i;
// 	int idx;
//
// 	i = 0;
// 	while (data->keys[i])
// 	{
// 		idx = poly_r_hash(data->keys[i]->key, VAR_BUFF);
// 		if (data->uev[idx])
// 		{
// 			iter_table(data->uev[idx], &cal_lvals, data, data->keys[i]);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

// calculate length of values and sum them to kv->var
// also add the values to the keys parsed from the input string
// void	cal_lvals(void *d, t_key_val *keys, t_data *data)
// {
// 	t_var_tb	*kv; 
// 	kv = (t_var_tb *)d;
// 	if (ft_strncmp(keys->key, kv->key, ft_strlen(keys->key) + 1) == 0)
// 	{
// 		data->lvals += ft_strlen(kv->var);
// 		keys->val = kv->var;
// 	}
// }


// void	store_key(int step, int i, char *input, t_data *data)
// {
// 	int	j;
// 	
// 	data->keys[data->key_iter] = malloc(sizeof(t_key_val));
// 	data->keys[data->key_iter]->key = malloc((step + 1) * sizeof(char));
// 	data->keys[data->key_iter]->val = NULL;
// 	i -= step;
// 	j = 0;
// 	while (j < step)
// 	{
// 		data->keys[data->key_iter]->key[j] = input[i];
// 		i++;
// 		j++;
// 	}
// 	data->keys[data->key_iter]->key[j] = '\0';
// 	//data->lvars += (step + 1);
// 	data->key_iter++;
// }

// // parses out environment variable names as keys
// void	print_keys(t_data *data)
// {
// 	int	i;
//
// 	i = 0;
// 	while (data->keys[i])
// 	{
// 		printf("key: %s\n", data->keys[i]->key);
// 		i++;
// 	}
// }
