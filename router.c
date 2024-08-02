#include "minishell.h"


// FREE THE TWO NEW SETS HERE, TEMPORARILY, OR
// WHEREVER DATA->TOK IS BEING FREED
//
// INSTEAD OF PASSING ALL THESE UGLY TRIPPLE
// POINTERS AROUND JUST PASS IN DATA AND 
// DO THE ARITHMETIC ON THE POINTER IN THE
// STRUCT

// HANDLE $( and = (
// 


void	sortin(t_data *data)
{
	char	**cpytok;
	t_args	**cpyset;

	cpyset = init_p_cmd_set(data->tok, data);
	data->redir = NULL;
	cpytok = data->tok;

	data->rtr_i = 0;
	while (*data->tok)
	{

		// check ([{
		if (m_set((*data->tok)[0], "("))   // MACRO SPECIAL
			handle_parens(data);	
			// filter for ( -> remove brackets and send to args (first
			// 		arg minishell
			// filter for " -> if no $ remove and send to args

		// check is direction
		if (m_set((*data->tok)[0], DIRECTIONS))
			add_direction(&data->tok, data);

		// if not a direction send to args
		else
			add_p_cmd_set(data);

		data->tok++;
	}		
	
	// does this work in all circumstancs? Yes? -> the last token must be a non pipe	
	data->p_cmd_set++;
	//*data->p_cmd_set = NULL;

	data->tok = cpytok;
	data->p_cmd_set = cpyset;

	// ft_printf("-------directions------\n");
	// printredlist(data->redir);
	// ft_printf("----------args---------\n");
	// printcmdargs(data);
	// ft_printf("---------tokens---------\n");

// 	check =
//	if (ft_strchr(data->tok[i], (int)'='))
		// set cmd to uservar
		// set ar[0] to var name (key)
		// set ar[1] to value
}

char	**trim_qts_varexp(t_data *data, char **arg)
{
	int		i;
	int		len;
	char	*cpy;

	i = 0;
	while (arg[i])
	{
		if (arg[i][0] == '\'')	
		{
			len = ft_strlen(arg[i]);
			cpy = malloc((len + 1) * sizeof(char));
			//error
			ft_strlcpy(cpy, arg[i], len + 1);
			//error
			free(arg[i]);
			arg[i] = malloc((len - 1) * sizeof(char));
			//error
			arg[i] = ft_strtrim(cpy, "\'");
			//error
			free(cpy);
		}
		else if (arg[i][0] == '\"')
		{
			len = ft_strlen(arg[i]);
			cpy = malloc((len + 1) * sizeof(char));
			//error
			ft_strlcpy(cpy, arg[i], len + 1);
			//error
			free(arg[i]);
			arg[i] = malloc((len - 1) * sizeof(char));
			//error
			arg[i] = ft_strtrim(cpy, "\"");
			//error
			ft_printf("..:%s\n", arg[i]);
			free(cpy);
			if (is_var(arg[i], '$'))
				expand_envv(data, arg[i]);
		}
		i++;
	}
	return (arg);
}

char	*handle_parens(t_data *data)
{
	int		i;
	int		j;
	int		len;
	char	*cpy;

	// the following parsing should happen before passing 
	// to execve in pipes otherwise no way to tell if 
	// literal or non literal string

	// if ((*data->tok)[0] == '\'')
	// {
	// 	len = ft_strlen(*data->tok);
	// 	cpy = malloc((len + 1) * sizeof(char));
	// 	ft_strlcpy(cpy, *data->tok, len + 1);
	// 	free(*data->tok);
	// 	*data->tok = malloc((len - 1) * sizeof(char));
	// 	*data->tok = ft_strtrim(cpy, "\'");
	// 	free(cpy);
	// }
	
	// bracket removal can happen here since cmd set to
	// minishell will always receive one argument of
	// type literal str

	// extra space in ->arg for minishell created in init
	// here minishell added to arg[0] index incremented
	// and outer brackets of token removed
	if ((*data->tok)[0] == '(')
	{
		(*data->p_cmd_set)->arg[data->rtr_i] = data->blt[0];
		data->rtr_i++;
		len = ft_strlen(*data->tok);
		cpy = malloc((len + 1) * sizeof(char));
		ft_strlcpy(cpy, *data->tok, len + 1);
		free(*data->tok);
		*data->tok = malloc((len - 1) * sizeof(char));
		i = 0;
		j = 0;
		while (cpy[i])
		{
			if (i > 0 && i < len - 1)
			{
				(*data->tok)[j] = cpy[i];
				j++;
			}
			i++;
		}
		(*data->tok)[j] = '\0';
		free(cpy);	
	}
	return (*data->tok);
}

void	add_p_cmd_set(t_data *data)
{
	if ((*data->tok)[0] == '|')
	{	
		(*data->p_cmd_set)->arg[data->rtr_i] = NULL;
		data->rtr_i = 0;
	}
	else
	{
		(*data->p_cmd_set)->arg[data->rtr_i] = *data->tok;
		data->rtr_i++;
		// perhaps better to set all to NULL whe
		// initialising then last and any remaining
		// will be null
		//if (!(data->tok + 1))
		//	(*data->p_cmd_set)->arg[data->rtr_i] = NULL;
	}
}

// not using tokens here
t_args	**init_p_cmd_set(char **tokens, t_data *data)
{
	int	i;
	int	j;
	int	n;
	int	f;

	i = 0;
	n = 0;
	while (data->tok[i])
	{
		if (data->tok[i][0] == '|')
			n++;
		i++;
	}
	data->p_cmd_set = ft_calloc(n + 2, sizeof(t_args *));
	if (!data->p_cmd_set)
		return (NULL);
	i = 0;
	j = 0;
	n = 0;
	while (data->tok[i])
	{
		if (data->tok[i][0] == '(')
			n++;
		if (data->tok[i][0] == '|' || !data->tok[i + 1])
		{
			data->p_cmd_set[j] = malloc(sizeof(t_args));
			data->p_cmd_set[j]->arg = malloc((n + 2) * sizeof(char *));
			if (!data->p_cmd_set[j]->arg)
				return (NULL);
			f = 0;
			while (f < (n + 2))
			{
				data->p_cmd_set[j]->arg[f] = NULL;
				f++;
			}
			n = 0;
			j++;
		}
		i++;
		n++;
	}
	data->p_cmd_set[j] = NULL;
	return (data->p_cmd_set);
}

// can replace token here with *data->tok
int	add_direction(char ***token, t_data *data)
{
	if (ft_strlen(**token) == 1)
	{
		if (ft_strncmp(**token, "|", 2) == 0)
			appendpipe(token, data);
		else if (ft_strncmp(**token, "<", 2) == 0)
			appendfunnel(token, data);
		else if (ft_strncmp(**token, ">", 2) == 0)
			appendfunnel(token, data);
	}	
	else
	{
		if (ft_strncmp(**token, ">>", 3) == 0)
			appendfunnel(token, data);
		if (ft_strncmp(**token, "<<", 3) == 0)
			appendfunnel(token, data);
	}	
	return (0);
}

// again replace token with *data->tok
int	appendpipe(char ***token, t_data *data)
{
	if (!data->redir)
		data->redir = create_red_node(**token);
	else
		appnd_red_list(&data->redir, **token);
	add_p_cmd_set(data);
	data->p_cmd_set++;
	return (0);
}

// again replace token with *data->tok
int	appendfunnel(char ***token, t_data *data)
{
	if (!data->redir)
	{
		data->redir = create_red_node(**token);
		(*token)++;
		appnd_red_list(&data->redir, **token);
	}
	else
	{
		appnd_red_list(&data->redir, **token);
		(*token)++;
		appnd_red_list(&data->redir, **token);
	}
	return (0);
}

void	appnd_red_list(t_red **node, char *dir)
{
	t_red	*ptr;

	ptr = *node;
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = create_red_node(dir);
}

t_red	*create_red_node(char *dir)
{
	t_red	*node;
	node = malloc(sizeof(t_red));
//	if (!node)
//		return (persub());
	node->dir = dir;
	node->next = NULL;
	return (node);
}

void	printredlist(t_red *node)
{
	while (node)
	{
		ft_printf("%s\n", node->dir);
		node = node->next;
	}
}

void	printcmdargs(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (data->p_cmd_set[i])
	{
		ft_printf("<");
		j = 0;
		while (data->p_cmd_set[i]->arg[j])
		{
			ft_printf(".> %s <", data->p_cmd_set[i]->arg[j]);
			j++;
		}
		ft_printf(".>");
		ft_printf("\n");
		i++;
	}
}

