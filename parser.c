/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:50:35 by linux             #+#    #+#             */
/*   Updated: 2024/06/23 17:21:13 by linux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Compile and test with:
// cc -Wall -Wextra -Werror parser.c error_handling.c string_handling.c\
// -L libft -lft -o parse
// $./parse

void	check_start(char **tokens)
{
	char	*word;

	if (ft_strchr(INVALID_START, **tokens))
	{
		word = get_word(*tokens);
		syntax_error(word, 2);
	}
}

int	parse_tokens(char **tokens)
{
	if (!*tokens)
		return (0);
	if (!**tokens || **tokens == '\0')
		return (0);
	check_start(tokens);
	// while (*tokens)
	// {
	// 	while (**tokens)
	// 	{
	// 		check_stuff();
	// 		**tokens++;
	// 	}
	// 	check_other_stuff();
	// 	*tokens++;
	// }
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
