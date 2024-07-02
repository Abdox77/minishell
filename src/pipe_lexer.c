/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 18:31:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/07/02 13:22:51 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	place_pipe_node(t_token **root, t_token **to_put)
{
	t_token	*tmp;

	if (!*root)
		*root = *to_put;
	else if (*root && ((*root)->type == AND || (*root)->type == OR)
		&& (*root)->was_in_parenthesis == FALSE)
	{
		tmp = (*root)->r_token;
		(*root)->r_token = *to_put;
		(*root)->r_token->l_token = tmp;
	}
	else
	{
		tmp = *root;
		*root = *to_put;
		(*root)->l_token = tmp;
	}
}

void	handle_pipe(t_token **token, char **line)
{
	*token = set_pipe_head();
	if (!*token)
		ft_error("failed to create token\n", EXIT_FAILURE);
	++(*line);
	lexer(token, line);
}

void	handle_pipes(t_token **token, char **line)
{
	t_token	*tmp;

	tmp = new_token(PIPE);
	(*line) += 1;
	place_node(token, &tmp, PIPE);
}
