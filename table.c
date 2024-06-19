#include "minishell.h"

// dictionary to store local user defined variables

t_var_tb	*create_node(char *key, char *var)
{
	t_var_tb	*node;

	node = malloc(sizeof(t_var_tb));
	node->key = NULL;
	node->var = NULL;
	if (!node)
		return (NULL);
	wrt_to_str(key, &node->key);
	wrt_to_str(var, &node->var);
	node->next = NULL;
	return (node);
}

int	add_var(t_var_tb **head, t_var_tb *node)
{
	t_var_tb	*ptr;

	if (*head == NULL)
	{
		*head = node;
		return (0);
	}
	ptr = *head;
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = node;
	return (0);
}

int	iter_table(t_var_tb *node, void (f)(void *, t_key_val *, t_data *), t_data *data, t_key_val *keys)
{
	if (!node)
		return (1);
	while (node->next != NULL)
	{
		f(node, keys, data);
		node = node->next;
	}
	f(node, keys, data);
	return (0);
}

void	print_node(void *node, t_data *data)
{
	t_var_tb	*tnode;
	tnode = (t_var_tb *)node;
	printf("%s: %s\n", tnode->key, tnode->var);
}

void	free_table(t_var_tb **node)
{
	t_var_tb	*tnode;
	t_var_tb	*ttnode;

	tnode = *node;
	while (tnode != NULL)
	{
		ttnode = tnode;
		tnode = tnode->next;
		free(ttnode->key);
		free(ttnode->var);
		free(ttnode);
	}
}

int	wrt_to_str(char *src, char **dst)
{
	int	i;

	*dst = malloc((ft_strlen(src) + 1) * sizeof(char));
	if (!dst)
		return (1);
	i = 0;
	while (src[i])
	{
		(*dst)[i] = src[i];
		i++;
	}
	(*dst)[i] = '\0';
	return (0);
}


