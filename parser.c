/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 15:50:35 by linux             #+#    #+#             */
/*   Updated: 2024/07/07 22:04:20 by linux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Compile and test with:
// cc -Wall -Wextra -Werror parser.c error_handling.c string_handling.c\
// -L libft -lft -o parse
// $./parse

void	check_parens(t_pdata *pdata)
{
	int	i;
	int	s;

	i = 0;
	while (pdata->current[i])
	{
		if (ft_strchr((const char *)"{[(", (int)pdata->current[i]))
		{
			if (pdata->parens.full)
				stack_error(pdata, 7);
			else
				p_push(&pdata->parens, pdata->current[i]);
		}
		else if (ft_strchr((const char *)")]}", (int)pdata->current[i]))
		{
			s = pdata->parens.top;
			if (pdata->parens.top == 0)
				stack_error(pdata, 7);
			else if (!p_match(pdata->parens.stack[s], pdata->current[i]))
				parens_error(pdata, 9);
			else
				(void)p_pop(&pdata->parens);
		}
		i++;
	}
	if (pdata->parens.top)
		parens_error(pdata, 9);
}

void	handle_redirection(t_pdata *pdata)
{
	if (!pdata->current && pdata->last && ft_strchr((const char *)"|<>",
			(int)*pdata->last))
		syntax_error(pdata, 5);
	if (pdata->last)
	{
		if (ft_strchr((const char *)"|<>", (int)*pdata->last))
			syntax_error(pdata, 5);
	}
}

void	handle_pipes(t_pdata *pdata)
{
	if (!pdata->current && pdata->last && '|' == *pdata->last)
		syntax_error(pdata, 6);
	if (ft_strlen(pdata->current > 1))
	{
		if ('|' == pdata->current[0] && '|' == pdata->current[1])
		{
			if (pdata->last && ft_strchr((const char *)"|<>",
					(int)*pdata->last))
				syntax_error(pdata, 6);
		}
	}
	else if (pdata->last)
	{
		if (*pdata->last == '|' || *pdata->last == '<' | *pdata->last == '>')
			syntax_error(pdata, 6);
	}
}

void	handle_quotes(t_pdata *pdata)
{
	int		i;
	char	*word;

	i = 0;
	while (pdata->current[i])
	{
		if ('\'' == pdata->current[i])
		{
			if (pdata->sqts && pdata->sqt_toggle)
				pdata->sqts--;
			else
				pdata->sqts++;
			pdata->sqt_toggle = !pdata->sqt_toggle;
			i++;
		}
		else if ('\"' == pdata->current[i])
		{
			if (pdata->dqts && !pdata->sqt_toggle)
				pdata->dqts--;
			else
				pdata->dqts++;
			pdata->sqt_toggle = !pdata->sqt_toggle;
			i++;
		}
	}
	if (pdata->sqts || pdata->dqts)
		quote_error(pdata, 3);
}

void	update_token(t_pdata *pdata, char *token)
{
	if (pdata->last)
		free(pdata->last);
	if (pdata->current)
		pdata->last = pdata->current;
	pdata->current = NULL;
	if (token && *token)
		pdata->current = ft_strdup((const char *)token);
}

initialize_pdata(t_pdata *pdata)
{
	pdata->sqt_toggle = false;
	pdata->sqts = 0;
	pdata->dqts = 0;
	pdata->last = NULL;
	pdata->current = NULL;
	pdata->parens.top = 0;
	pdata->parens.full = false;
}

void	check_start(t_pdata *pdata)
{
	if (ft_strchr(INVALID_START, *pdata->current))
		syntax_error(pdata, 2);
}

// the entries of quotes[3] are: toggle, single, double
int	parse_tokens(char **tokens)
{
	int		i;
	t_pdata	pdata;

	if (!tokens || !*tokens || !**tokens || **tokens == '\0')
		return (0);
	i = 0;
	initialize_pdata(&pdata);
	update_token(&pdata, tokens[i]);
	check_start(&pdata);
	while (tokens[i])
	{
		update_token(&pdata, tokens[i]);
		if ("\"" == *pdata.current || "\'" == *pdata.current)
			handle_quotes(&pdata);
		else if ('|' == *pdata.current)
			handle_pipes(&pdata);
		else if ('<' == *pdata.current || '>' == *pdata.current)
			handle_redirection(&pdata);
		else
			check_parens(&pdata);
		i++;
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
