/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+	+#+           */
/*   Created: 2024/06/21 15:50:35 by linux             #+#    #+#             */
/*   Updated: 2024/07/29 21:48:31 by lworden          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Compile and test with:
// cc -g -Wall -Wextra -Werror parser.c error_handling.c parens_stack.c
// string_handling.c -L libft -lft -o pppp
// $./pppp

void	check_parens(t_pdata *pdata)
{
	int i;
	int s;

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

bool	handle_tail(t_pdata *pdata)
{
	if (!(pdata->current) && pdata->last && ft_strchr((const char *)"&|<>",
			(int)*pdata->last))
		syntax_error(pdata, 5);
	if (pdata->last)
	{
		if (ft_strchr((const char *)"&|<>", (int)*pdata->last))
			syntax_error(pdata, 5);
	}
	return (false);
}

void	handle_redirection(t_pdata *pdata)
{
	if (!(pdata->current) && pdata->last && ft_strchr((const char *)"|<>",
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
	if (ft_strlen(pdata->current) > 1)
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
		if (*pdata->last == '|' || *pdata->last == '<' || *pdata->last == '>')
			syntax_error(pdata, 6);
	}
}

// void	handle_quotes(t_pdata *pdata)
// {
// 	int i;

// 	i = 0;
// 	while (pdata->current[i])
// 	{
// 		if ('\'' == pdata->current[i])
// 		{
// 			if (pdata->sqts == 0 && pdata->sqt_toggle <= 0)
// 			{
// 				pdata->sqts++;
// 				pdata->sqt_toggle = 1;
// 			}
// 			else if (pdata->sqts && pdata->sqt_toggle == 0)
// 			{
// 				pdata->sqts++;
// 				pdata->sqt_toggle = 1;
// 			}
// 			else if (pdata->sqts && pdata->sqt_toggle > 0)
// 			{
// 				pdata->sqts--;
// 				pdata->sqt_toggle = 0;
// 				if (!pdata->sqts && !pdata->dqts)
// 					pdata->sqt_toggle = -1;
// 			}
// 			else
// 				quote_error(pdata, 3);
// 		}
// 		else if ('\"' == pdata->current[i])
// 		{
// 			if (pdata->dqts == 0 && pdata->sqt_toggle != 0)
// 			{
// 				pdata->dqts++;
// 				pdata->sqt_toggle = 0;
// 			}
// 			else if (pdata->dqts && pdata->sqt_toggle > 0)
// 			{
// 				pdata->dqts++;
// 				pdata->sqt_toggle = 0;
// 			}
// 			else if (pdata->dqts && pdata->sqt_toggle == 0)
// 			{
// 				pdata->dqts--;
// 				pdata->sqt_toggle = 1;
// 				if (!pdata->sqts && !pdata->dqts)
// 					pdata->sqt_toggle = -1;
// 			}
// 			else
// 				quote_error(pdata, 3);
// 			// if (pdata->dqts && !pdata->sqt_toggle)
// 			// 	pdata->dqts--;
// 			// else
// 			// 	pdata->dqts++;
// 			// pdata->sqt_toggle = !pdata->sqt_toggle;
// 		}
// 		i++;
// 	}
// 	if (pdata->sqts || pdata->dqts)
// 		quote_error(pdata, 3);
// }

void	handle_quotes(t_pdata *pdata)
{
	int i;

	i = 0;
	if (pdata->current && pdata->current[i] && '\'' == pdata->current[i])
	{
		i++;
		pdata->sqts++;
		pdata->sqt_toggle = 1;
		while ('\0' != pdata->current[i] && '\'' != pdata->current[i])
			i++;
		if ('\'' == pdata->current[i])
		pdata->sqts--;
		if (pdata->sqts)
			quote_error(pdata, 3);
		pdata->sqt_toggle = -1;
	}
	else if (pdata->current && pdata->current[i] && '\"' == pdata->current[i])
	{
		i++;
		pdata->dqts++;
		pdata->sqt_toggle = 0;
		while ('\0' != pdata->current[i] && '\"' != pdata->current[i])
			i++;
		if ('\"' == pdata->current[i])
			pdata->dqts--;
		if (pdata->dqts)
			quote_error(pdata, 3);
		pdata->sqt_toggle = -1;
	}
}

void	update_token(t_pdata *pdata, char *token)
{
	if (pdata->last)
	{
		free(pdata->last);
		pdata->last = NULL;
	}
	if (pdata->current)
	{
		pdata->last = ft_strdup((const char *)pdata->current);
		free(pdata->current);
		pdata->current = NULL;
	}
	if (token) // && ft_isprint((int)*token))
		pdata->current = ft_strdup((const char *)token);
}

void	initialize_pdata(t_pdata *pdata)
{
	pdata->sqt_toggle = -1;
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
	int i;
	t_pdata pdata;

	if (!tokens || !*tokens)
		return (0);
	else if (!**tokens || **tokens == '\0')
		return (0);
	i = 0;
	initialize_pdata(&pdata);
	update_token(&pdata, tokens[i]);
	check_start(&pdata);
	while (tokens[i])
	{
		if (pdata.current == NULL)
		{
			if (!handle_tail(&pdata))
				break ;
		}
		else if (!*pdata.current)
		{
			if (!handle_tail(&pdata))
				break ;
		}
		else
		{
			if ('\"' == *pdata.current || '\'' == *pdata.current)
				handle_quotes(&pdata);
			else if ('|' == *pdata.current)
				handle_pipes(&pdata);
			else if ('<' == *pdata.current || '>' == *pdata.current)
				handle_redirection(&pdata);
			else
				check_parens(&pdata);
			if (tokens[i + 1])
				update_token(&pdata, tokens[++i]);
			else
				update_token(&pdata, NULL);
		}
		if (!pdata.current && !pdata.last)
			return (0);
	}
	return (0);
}

#include <stdio.h>


//int	main(void)
//{
//	int res;
//	char **tokens;
//
//
//	char *good_commands[][10] = {{"echo", "\"Hello, World!\"", NULL},
//									{"ls", "-l", "/home/user", NULL},
//									{"ls", ">", "outfile.txt", NULL},
//									{"cat", "<", "infile.txt", NULL},
//									{"ls", ">>", "outfile.txt", NULL},
//									{"cat", "<<", "infile.txt", NULL},
//									{"grep", "\"search_term\"", "<", "infile.txt", ">", "outfile.txt", NULL},
//									{"ls", "|", "grep", "\"search_term\"", NULL},
//									{"cat", "file.txt", "|", "wc", "-l", NULL},
//									{"mkdir", "new_dir", "&&", "cd", "new_dir", NULL},
//									{"touch", "file1", "&&", "echo", "\"content\"", ">", "file1", NULL},
//									{"false", "||", "echo","\"This runs because the previous command failed\"", NULL},
//									{"true", "||", "echo", "\"This won't run because the previous command succeeded\"", NULL},
//									{"(cd /tmp && ls)", NULL},
//									{"{echo \"Hello\" echo \"World\";}", NULL},
//									{"echo", "\"This is a string with spaces\"", NULL},
//									{"echo", "'This is another string with spaces'", NULL},
//									{"echo", "\"Mixing 'single' and \\\"double\\\" quotes\"", NULL},
//									{"echo", "\"Current date: $(date)\"", NULL},
//									{"echo", "\"Files: $(ls)\"", NULL},
//									{"echo", "\"Hello, World!\"", "&&", "ls", "-l", "|", "grep", "\"txt\"", NULL},
//									{"(echo \"Start\" && ls)", "||", "echo", "\"Failed\"", NULL},
//									{"{echo \"Block 1\"; echo \"Block 2\";}", ">", "blocks.txt", NULL},
//									{"echo", "\"$(date): File list\"", "&&", "ls", ">", "filelist.txt", NULL},
//									{NULL}};
//
//
//	// Bad commands
//	char *bad_commands[][10] = {
///*								{"echo", "\"This is an unmatched quote", NULL},
//								{"echo", "'This is another unmatched quote", NULL},
//								{"echo", "(unmatched", "parentheses", NULL},
//								{"echo", "unmatched", "parentheses)", NULL},
//								{"echo", "{unmatched", "braces", NULL},
//								{"echo", "unmatched", "braces}", NULL},
//								{"echo", "[unmatched", "brackets", NULL},
//								{"echo", "unmatched", "brackets]", NULL},
//								{"|", "echo", "\"Pipe at the beginning\"", NULL},
//								{"echo", "\"Pipe at the end\"", "|", NULL},
////								{">", "outfile.txt", "echo", "\"Redirection at the beginning\"", NULL},
//								{"echo", "\"Redirection at the end\"", ">", NULL},						// NOT AN ERROR CASE!
//								{"echo", "\"Double redirection\"", ">>", ">>", "outfile.txt", NULL},
//								{"&&", "echo", "\"Logical AND at the beginning\"", NULL},
//								{"echo", "\"Logical AND at the end\"", "&&", NULL},
//								{"||", "echo", "\"Logical OR at the beginning\"", NULL},
//								{"echo", "\"Logical OR at the end\"", "||", NULL},
//								{"echo", "*", "|", NULL},
////								{"echo", "|", "*", NULL}, 												// NOT AN ERROR CASE!
//								{"echo", "\"Mismatched quotes'", "&&", "ls", NULL},
//								{"(", "echo", "\"Missing closing parenthesis\"", "||", "ls", NULL},
//								{"echo", "\"Redirection in the middle\"", ">", "|", "grep", "\"error\"", NULL},
//								{"echo", "\"Logical AND at end\"", "&&", NULL},
////								{"echo", "\"Unmatched Q im cmd subst $(date && echo 'Done)\"", NULL},	// NOT AN ERROR CASE!
//*/								{NULL}};
//
//
//	printf("Parsing good commands:\n");
//	for (int i = 0; i < (int)(sizeof(good_commands)
//			/ sizeof(good_commands[0])); i++)
//	{
//		tokens = good_commands[i];
//		res = parse_tokens(tokens);
//		if (res == 0)
//			printf("SUCCESSFUL PARSE\n");
//	}
//
////	(void)good_commands;
////	(void)res;
//
//	printf("\nParsing bad commands:\n");
//	for (int i = 0; i < (int)(sizeof(bad_commands)
//			/ sizeof(bad_commands[0])); i++)
//	{
//		tokens = bad_commands[i];
//		parse_tokens(tokens);
//	}
//	return (0);
//}
