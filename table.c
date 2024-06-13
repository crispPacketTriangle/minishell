#include "minishell.h"

// dictionary to store local user defined variables

typedef struct t_var_tb
{
	char			*key;
	char			*var;
	struct t_var_tb	*next;
}	t_var_tb;

t_var_tb	*create_node(char *key, char *var)
{
	t_var_tb	*node;

	node = malloc(sizeof(t_var_tb));
	if (!node)
		return (NULL);
	node->key = key;
	node->var = var;
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

int	iter_table(t_var_tb *node, void (f)(void *))
{
	if (!node)
		return (1);
	while (node->next != NULL)
	{
		f(node);
		node = node->next;
	}
	f(node);
	return (0);
}

void	print_node(void *node)
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

void	wrt_to_str(char *src, char *dst)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

int	main()
{
	t_var_tb	*head;
	t_var_tb	*node1;
	char		*key1;
	char		*key2;
	char		*var1;
	char		*var2;

	key1 = malloc(11 * sizeof(char));
	wrt_to_str("NO_FKS_GVN", key1);
	key2 = malloc(11 * sizeof(char));
	wrt_to_str("NO_SCREENS", key2);
	var1 = malloc(33 * sizeof(char));
	wrt_to_str("a state of unruffled contentment", var1);
	var2 = malloc(6 * sizeof(char));
	wrt_to_str("sleep", var2);
	
	head = create_node(key1, var1);
	node1 = create_node(key2, var2);
	add_var(&head, node1);

	iter_table(head, &print_node);
	free_table(&head);
}


