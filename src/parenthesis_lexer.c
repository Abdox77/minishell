/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_lexer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 18:30:27 by amohdi            #+#    #+#             */
/*   Updated: 2024/07/02 13:15:33 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	place_parenthesis_node(t_token **root, t_token **to_put)
{
	if (!*root)
		*root = *to_put;
	else if (!(*root)->l_token)
		(*root)->l_token = *to_put;
	else if (!(*root)->r_token)
		(*root)->r_token = *to_put;
	else
		place_parenthesis_node(&((*root)->r_token), to_put);
}

static void	add_redirection_after_parenthsis(t_token **node, char **line)
{
	if (is_redirection_char(**line) == TRUE)
	{
		if (!*node)
			ft_print_error("Syntax error unexpected token near ')'\n", line,
				SAVE);
		if ((*node))
		{
			if ((*node)->type == CMD)
				handle_redirection(node, line, FALSE);
			else
				handle_redirection(node, line, TRUE);
		}
	}
	else if (is_special_char(**line) == FALSE && **line)
		ft_print_error("Syntax error unexpected token after ')'\n", line, SAVE);
}

void	handle_parenthesis(t_token **root, char **line)
{
	t_token	*node;

	++(*line);
	node = NULL;
	parenthesis_lexer(&node, line);
	if (!*line || **line != ')')
	{
		ft_print_error("Syntax error unlosed parenthesis\n", line, SAVE);
		return ;
	}
	++(*line);
	special_trim(line);
	while (is_redirection_char(**line) == TRUE)
	{
		add_redirection_after_parenthsis(&node, line);
		special_trim(line);
	}
	node->was_in_parenthesis = TRUE;
	place_node(root, &node, PARENTHESIS);
}

void	parenthesis_lexer(t_token **token, char **line)
{
	if (!*line || !**line || **line == ')')
		return ;
	special_trim(line);
	if (is_an_operator(line, 0) == TRUE)
		handle_operators(token, line);
	else if (**line == '|')
		handle_pipes(token, line);
	else if (**line == '(')
		handle_parenthesis(token, line);
	else
		handle_commands(token, line);
	parenthesis_lexer(token, line);
}
