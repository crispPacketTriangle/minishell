#include "minishell.h"

// todo: implement multiple exports or unsets on one line
// export var1=a val2=b val3=c
// or
// export var1 var2 var3
// the parser should handle all scenarios and so will
// hand off a struct with the arguments ready for processing
// which will first be checked for variable expansion
// then passed onto the relevant command

// split find_ent 

int	unb_export(t_args *args, t_data *data)
{
	int			i;
	int			idx;
	t_var_tb	*var;
	
	i = 1;
	while (args->arg[i])
	{
		var = NULL;
		idx = poly_r_hash(args->arg[i]);
		if (data->uev[idx])
			var = find_uev(data->uev[idx], args->arg[i]);
		if (!var)
			return (perrsub());
		if (data->ent[idx] == 0)
			data->ent[idx] = create_node(var->key, var->var);
		else
			add_var(&data->ent[idx], create_node(var->key, var->var));
		i++;
	}
	return (0);
}

t_var_tb	*find_uev(t_var_tb *node, char *key)
{
	while (node)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0)
				return (node);
		node = node->next;
	}
	return (NULL);
}

t_args	*init_export_test(char *input, t_data *data)
{
	int		i;
	t_args	*test;

	test = malloc(sizeof(t_args));
	test->arg = malloc((len_arr(data->tok) + 1) * sizeof(char *));
	//errors
	i = 0;
	while (data->tok[i])
	{
		test->arg[i] = data->tok[i];
		i++;
	}
	test->arg[i] = NULL; 
	return (test);
}

int	unb_unset(t_args *args, t_data *data)
{
	int			i;
	int			idx;
	
	i = 1;
	while (args->arg[i])
	{
		idx = poly_r_hash(args->arg[i]);
		if (data->ent[idx])
			find_ent(idx, args->arg[i], data);
		i++;
	}
	return (0);
}

// for testing loop to create a bunch of variables
// test for sandwiched vars by looping over array
// and printing first i and then everything in list
// try removing sandwiched values
// or else brute force to the point where sandwiched
// vars certainly exist in the array

// can one unset systen env vars like PATH in bash?
// probably yes
int	find_ent(int idx, char *key, t_data *data)
{
	t_var_tb	*prev;
	t_var_tb	*cur;
	t_var_tb	*copy;
	
	cur = data->ent[idx];
	prev = NULL;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key)) == 0)
		{
			if (cur->next == NULL) 			// last or only entry
			{
				free(cur->key);
				free(cur->var);
				cur = NULL;
				if (!prev)
					data->ent[idx] = NULL;
				else
					prev->next = NULL;
			}
			else if (prev == NULL) 	// is head
			{
				copy = cur->next;
				free(cur->key);
				free(cur->var);
				free(cur);
				data->ent[idx] = copy;
			}
			else							// is sandwiched
			{
				copy = cur->next;
				free(cur->key);
				free(cur->var);
				free(cur);
				prev->next = copy;  // it needs to be the og pointer
			}
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
