#include "minishell.h"

void	sortin(t_data *data)
//
{
	char		**cpytok;
	t_args	**cpyset;

	init_p_cmd_set(data->tok, data);
	data->redir = NULL;
	cpytok = data->tok;	
	while (*data->tok)
	{

		printf("token: %s\n", *data->tok);
// 	check ([{
//		if (m_set(data->tok[i][0], "\'\"([{"))
			// filter for ' -> remove quotes and send to args
			// filter for ( -> remove brackets and send to args
			// filter for { -> do we have to deal with curly braces?

// 	check is direction

		if (m_set((*data->tok)[0], DIRECTIONS))
			add_direction(&data->tok, data);

// if not a direction send to args
		else
			add_p_cmd_set(data->tok, data);

		data->tok++;
	}		



	printredlist(data->redir);
	ft_printf("-----------------------\n");	
	data->tok = cpytok;
	data->p_cmd_set = cpyset;

// 	check =
//	if (ft_strchr(data->tok[i], (int)'='))
		// set cmd to uservar
		// set ar[0] to var name (key)
		// set ar[1] to value


// if bool cmd
// 	set tok[i] to command
// 	set bool to ar

//	if ar
//		set tok[i] to ar[j]
//		j++;

}

void	add_p_cmd_set(char **data->tok, t_data *data)
{
	
}

t_args	*init_p_cmd_set(char	**data->tok, t_data *data)
{
	int	i;
	int	j;
	int	n;

	i = 0;
	n = 0;
	while (data->tok[i])
	{
		if (data->tok[i][0] == '|')
			n++;
	}
	data->p_cmd_set = malloc((n + 2) * sizeof(t_args *));
	if (!data->p_cmd_set)
		return (NULL);
	i = 0;
	j = 0;
	n = 0;
	while (data->tok[i])
	{
		if (data->tok[i][0] == '|')
		{
			data->p_cmd_set[j] = malloc((n + 1) * sizeof(char *));
			if (!data->p_cmd_set[j])
				return (NULL);
			n = 0;
			j++;
		}
		i++;
		n++;
	}
}




int	add_direction(char ***token, t_data *data)
{
	//loop is outside so have to pass in token pointer 
	//do pointer arithmetic

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

int	appendpipe(char ***token, t_data *data)
{
	if (!data->redir)
		data->redir = create_red_node(**token);
	else
		appnd_red_list(&data->redir, **token);
	return (0);
}

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
	t_red	*prv;

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

