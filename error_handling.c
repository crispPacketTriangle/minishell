/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:29:16 by linux             #+#    #+#             */
/*   Updated: 2024/06/23 17:13:19 by linux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	syntax_error(char *str, int n)
{
	char	*e1;

	e1 = "unbash: syntax error near unexpected token `";
	write(2, e1, ft_strlen(e1));
	ft_putstr_fd(str, 2);
	e1 = "'\n";
	write(2, e1, ft_strlen(e1));
	exit(n);
}

void	malloc_error(int e)
{
	write(2, "unbash: error: could not allocate memory", 40);
	exit(e);
}
