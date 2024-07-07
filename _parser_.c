/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:50:35 by linux             #+#    #+#             */
/*   Updated: 2024/07/07 19:37:55 by linux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Compile and test with:
// cc -Wall -Wextra -Werror parser.c error_handling.c string_handling.c\
// -L libft -lft -o parse
// $./parse

handle_redirection(char *current, int *redirect)
{
	int		i;
	char	*word;

	while (*current)
	{
		if (redirect[0] && redirect[1])
		{
			word = get_word(current);
			syntax_error(current, 5);
		}
		if ('<' == *current)
			redirect[0]++;
	}
}

bool	handle_quotes(char *current, int *quotes)
{
	int		i;
	char	*word;

	while (current[i])
	{
		if ('\'' == current[i])
		{
			if (quotes[1] && quotes[0])
				quotes[1]--;
			else
				quotes[1]++;
			quotes[0] = !quotes[0];
			i++;
		}
		else if ('\"' == *current)
		{
			if (quotes[2] && !quotes[0])
				quotes[2]--;
			else
				quotes[2]++;
			quotes[0] = !quotes[0];
			i++;
		}
	}
	if (quotes[1] || quotes[2])
	{
		word = get_word(current);
		quote_error(word, 3);
	}
	return (true);
}

void	check_start(char **tokens)
{
	char	*word;

	if (ft_strchr(INVALID_START, **tokens))
	{
		word = get_word(*tokens);
		syntax_error(word, 2);
	}
}

// the entries of quotes[3] are: toggle, single, double
int	parse_tokens(char **tokens)
{
	char	*current;
	int		*quotes;
	bool	pipe;
	int		redirect[2];
	char	*word;

	if (!tokens || !*tokens || !**tokens || **tokens == '\0')
		return (0);
	check_start(tokens);
	quotes = malloc(sizeof(int) * 3);
	if (!quotes)
		malloc_error(errno);
	quotes[0] = 0;
	quotes[1] = 0;
	quotes[2] = 0;
	pipe = false;
	redirect[0] = 0;
	redirect[1] = 0;
	while (*tokens)
	{
		current = *tokens;
		if ("\"" == *current || "\'" == *current)
			handle_quotes(current, quotes);
		else if (pipe && '|' == *current)
		{
			word = get_word(current);
			syntax_error(current, 2);
		}
		else if ('<' == *current || '>' == *current)
		{
			handle_redirection(current, redirect);
		}
		pipe = false;
		if ('|' == *current)
			pipe = true;
		current++;
		if (quotes[1] || quotes[2])
		{
			word = get_word(*tokens);
			quote_error(word, 2);
		}
		// 		check_stuff();
		// 		**tokens++;
		// 	}
		// 	check_other_stuff();
		*tokens++;
	}
	free(quotes);
	if (pipe)
	{
		word = get_word("|");
		syntax_error(word, 4);
	}
	return (0);
}

int	main(void)
{
	char	*token;
	char	**tokens;

	token = "} echo 'hello'";
	tokens = &token;
	parse_tokens(tokens);
	return (0);
}
