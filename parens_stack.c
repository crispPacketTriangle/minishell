#include "minishell.h"

void	p_push(t_stack *s, char c)
{
	s->stack[++(s->top)] = c;
}

char	p_pop(t_stack *s)
{
	return (s->stack[(s->top)--]);
}

bool	p_match(char open, char close)
{
	return (open == '(' && close == ')') || (open == '{' && close == '}')
		|| (open == '[' && close == ']');
}
