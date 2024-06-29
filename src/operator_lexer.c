/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_lexer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 18:32:18 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 18:33:57 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_eval  check_operator_syntax(char **line)
{
	char	operator;

	if (!**line)
		return (ft_print_error("Syntax error unexpected token near operator\n",
				line, SAVE), FAILED);
	operator = **line;
	++(*line);
	if (!**line)
		return (ft_print_error("Syntax error unexpected token near operator\n",
				line, SAVE), FAILED);
	if (**line == operator)
	{
		++(*line);
		return (PASSED);
	}
	ft_print_error("Syntax error unexpected token near operator\n", line, SAVE);
	return (FAILED);
}

void	place_operator_node(t_token **root, t_token **to_put)
{
	t_token	*tmp;

	if (!*to_put)
		return ;
	else if (!*root)
		*root = *to_put;
	else
	{
		tmp = *root;
		*root = *to_put;
		(*root)->l_token = tmp;
	}
}

t_bool	handle_error_if_not_valid_op(char **line, char operator, int len)
{
	while ((*line)[len + 1] && is_space((*line)[len + 1]))
		len++;
	if ((*line)[len + 1] == '|' || (*line)[len + 1] == '&')
	{
		if (operator == '|')
			ft_print_error("Syntax error unexpected token near '|'\n",
				line, SAVE);
		else
			ft_print_error("Syntax error unexpected token near '&'\n",
				line, SAVE);
		return (TRUE);
	}
	return (FALSE);
}

t_bool	is_an_operator(char **line, int len)
{
	char	operator;

	if (((*line)[len] == '&' || (*line)[len] == '|'))
	{
		operator = (*line)[len];
		if ((*line)[len + 1] == operator)
			return (TRUE);
		if (operator == '&')
			return (ft_print_error("Syntax error unexpected token near '&'\n",
					line, SAVE), TRUE);
		else
			return(handle_error_if_not_valid_op(line, operator, len));
	}
	return (FALSE);
}

void	handle_operators(t_token **token, char **line)
{
	t_token	*tmp;
	char	operator;

	operator = **line;
	if (check_operator_syntax(line) == FAILED)
		return ;
	if (operator == '&')
		tmp = new_token(AND);
	else
		tmp = new_token(OR);
	place_node(token, &tmp, tmp->type);
}
