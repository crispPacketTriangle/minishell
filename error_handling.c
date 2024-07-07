/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:29:16 by linux             #+#    #+#             */
/*   Updated: 2024/07/07 22:02:40 by linux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"

void	free_data(t_pdata *pdata)
{
	free(pdata->current);
	pdata->current = NULL;
	free(pdata->last);
	pdata->last = NULL;
}

void	syntax_error(t_pdata *pdata, int n)
{
	char	*e1;

	e1 = "unbash: syntax error near unexpected token `";
	write(2, e1, ft_strlen(e1));
	ft_putstr_fd(pdata->current, 2);
	e1 = "'\n";
	write(2, e1, ft_strlen(e1));
	free_data(pdata);
	exit(n);
}

void	quote_error(t_pdata *pdata, int n)
{
	char	*e1;

	e1 = "unbash: syntax error--unmatched quote before or near token `";
	write(2, e1, ft_strlen(e1));
	ft_putstr_fd(pdata->current, 2);
	e1 = "'\n";
	write(2, e1, ft_strlen(e1));
	free_data(pdata);
	exit(n);
}

void	parens_error(t_pdata *pdata, int n)
{
	char	*e1;

	e1 = "unbash: syntax error--unmatched backet before or near token `";
	write(2, e1, ft_strlen(e1));
	ft_putstr_fd(pdata->current, 2);
	e1 = "'\n";
	write(2, e1, ft_strlen(e1));
	free_data(pdata);
	exit(n);
}

void	stack_error(t_pdata *pdata, int n)
{
	char	*e1;

	e1 = "unbash: stack overflow or underflow near token `";
	write(2, e1, ft_strlen(e1));
	ft_putstr_fd(pdata->current, 2);
	e1 = "'\n";
	write(2, e1, ft_strlen(e1));
	free_data(pdata);
	exit(n);
}

void	malloc_error(int e)
{
	write(2, "unbash: error: could not allocate memory", 40);
	exit(e);
}
