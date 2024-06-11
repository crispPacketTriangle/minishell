#include "minishell.h"

// dictionary to store local user defined variables

typedef struct t_var_tb
{
	const char			*key;
	const char			*var;
	struct t_var_tb	*next;
}	t_var_tb;

t_var_tb	*create_node(const char *key, const char *var)
{
	t_var_tb	*node;

	node = malloc(sizeof(t_var_tb));
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

int	main()
{
	t_var_tb *head;
	t_var_tb *node1;

	head = create_node("NO_FKS_GVN", "a state of unruffled contentment");
	node1 = create_node("NO_SCREENS", "sleep");
	add_var(&head, node1);

	iter_table(head, &print_node);
}
