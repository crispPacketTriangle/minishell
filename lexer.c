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
		if (m_set(input[i], QTS))
			mid(input[i], data);
		if (m_set(input[i], PARENS))
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
// 
// TODO:
// 
// tidy up function, add variable expansion for strings
// that contain other chars
int	expand_envv(t_data *data, char *str)
{
	int		idx;

	// if (!is_var(data, str, '$'))
	// 	return (0);
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

int	is_var(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

// int	is_var(t_data *data, char *str, char c)
// {
// 	int	i;
//
// 	if (m_set(str[0], STR_LIT_START))
// 		return (0);
// 	init_qts(data, 0);
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (m_set(str[i], QTS))
// 			mid(str[i], data);
// 		if (m_set(str[i], PARENS))
// 			parens(str[i], data);
// 		if (str[i] == c && data->sqts == 0 && data->paren == 0)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }


// calculate length of values and sum them to kv->var
// also add the values to the keys parsed from the input string
void	expand_var(void *d, t_data *data, char *str)
{
	t_var_tb	*kv;
	int			len;

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
