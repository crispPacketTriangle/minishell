#include "minishell.h"

int	pipes()
{
	return (0);	
}

// need a way to split the tokenised char ** array
//

int	token_split(char **tokens, t_data *data)
{
	int	i;
	
	i = 0;
	while (tokens[i])
	{
		if (ft_strlen(tokens[i]) == 1 && tokens[i][0] == '|')

	}
	return (0);
}

int	init_args(char **tokens, t_data *data)
{
	data->args = malloc((data->n_p + 1) * sizeof(t_args *));
	token_split(tokens, data);
	return (0);
}
