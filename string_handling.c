/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:32:00 by linux             #+#    #+#             */
/*   Updated: 2024/06/23 17:20:18 by linux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_len(char *str)
{
	int	i;

	i = 0;
	while (*str && *str != ' ')
	{
		str++;
		i++;
	}
	return (i);
}

char	*get_word(char *str)
{
	size_t	len;
	char	*word;

	len = get_len(str);
	word = malloc((len + 1) * sizeof(char));
	if (!word)
	{
		word = NULL;
		malloc_error(*str);
	}
	ft_memcpy((void *)word, (const void *)str, len);
	word[len] = '\0';
	return (word);
}
