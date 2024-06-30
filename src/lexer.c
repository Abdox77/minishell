/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 13:53:46 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	place_cmd_node(t_token **root, t_token **to_put)
{
	if (!*root)
		*root = *to_put;
	else if ((*root)->type != CMD)
	{
		if (!((*root)->l_token))
			(*root)->l_token = *to_put;
		else if (!((*root)->r_token))
			(*root)->r_token = *to_put;
		else
			place_cmd_node(&((*root)->r_token), to_put);
	}
}

void	place_node(t_token **root, t_token **to_put, enum e_token type)
{
	if (type == CMD)
		place_cmd_node(root, to_put);
	else if (type == AND || type == OR)
		place_operator_node(root, to_put);
	else if (type == PIPE)
		place_pipe_node(root, to_put);
	else if (type == PARENTHESIS)
		place_parenthesis_node(root, to_put);
}

void	handle_commands(t_token **root, char **line)
{
	t_token	*tmp;

	tmp = handle_command(line);
	place_node(root, &tmp, CMD);
}

void	lexer(t_token **token, char **line)
{
	if (!*line || !**line)
		return ;
	special_trim(line);
	if (is_an_operator(line, 0) == TRUE)
		handle_operators(token, line);
	else if (**line == '|')
		handle_pipes(token, line);
	else if (**line == ')')
		ft_print_error("Syntax error unexpected token near ')'\n", line, SAVE);
	else if (**line == '(' || **line == ')')
		handle_parenthesis(token, line);
	else
		handle_commands(token, line);
	lexer(token, line);
}
t_token	*lexer_manager(char **line)
{
	if (!*line || !**line)
		return NULL; 
    t_token *head;

    head = NULL;
    lexer(&head, line);
    if (!head)
        ft_print_error("Syntax Error\n", line, SAVE);
    return head;
}
