/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:01:01 by linux             #+#    #+#             */
/*   Updated: 2024/06/23 17:12:06 by linux            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# define INVALID_START ";|&)}]"

void	check_start(char **tokens);
int		parse_tokens(char **tokens);
char	*get_word(char *str);
void	syntax_error(char *str, int n);
void	malloc_error(int e);

#endif
