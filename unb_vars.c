#include "minishell.h"

int	init_en_var_table(char **env, t_data *data)
{

	data->ent = malloc(VAR_BUFF * sizeof(t_var_tb *));
	if (!data->ent)
		return (1);
	ft_memset(data->ent, 0, VAR_BUFF);
	load_env_vars(env, data);
	return (0);
}

int	load_env_vars(char **env, t_data *data)
{
	int			i;
	int			idx;
	t_key_val	temp;

	i = 0;
	while (env[i])
	{
		extract(&temp, env[i]);
		idx = poly_r_hash(temp.key);
		if (data->ent[idx] == 0)
			data->ent[idx] = create_node(temp.key, temp.val);
		else
			add_var(&data->ent[idx], create_node(temp.key, temp.val));
		free(temp.key);
		free(temp.val);
		i++;
	}
	return (0);
}

int	extract(t_key_val *temp, char *var)
{
	int	i;
	int	dlim;
	int	len;

	dlim = 0;
	while (var[dlim])
	{
		if (var[dlim] == '=')
			break;
		dlim++;
	}
	temp->key = malloc((dlim + 1) * sizeof(char));
	//errors
	i = 0;
	while (i < dlim)
	{
		temp->key[i] = var[i];
		i++;
	}
	temp->key[i] = '\0';
	dlim++;
	i = 0;
	len = ft_strlen(var);
	temp->val = malloc(((len - dlim) + 1) *sizeof(char));
	//errors
	while (dlim < len)
	{
		temp->val[i] = var[dlim];
		i++;
		dlim++;
	}
	temp->val[i] = '\0';
	//printf("key: %s\nval: %s\n", temp->key, temp->val);
	return (0);
}

int	unb_export(t_args *args, t_data *data)
{
	int			idx;
	t_var_tb	*var;
	
	var = NULL;
	idx = poly_r_hash(args->arg[0]);
	if (data->uev[idx])
		var = find_uev(data->uev[idx], args->arg[0]);
	if (!var)
		return (perrsub());
	if (data->ent[idx] == 0)
		data->ent[idx] = create_node(var->key, var->var);
	else
		add_var(&data->ent[idx], create_node(var->key, var->var));
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
	t_args	*test;

	test = malloc(sizeof(t_args));
	test->cmd = data->tok[0];
	test->arg = &data->tok[1];
	return (test);
}
