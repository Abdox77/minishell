/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 18:31:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 18:56:30 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// not sure about the fix of this function 
void	place_pipe_node(t_token **root, t_token **to_put)
{
	t_token	*tmp;

	if (!*root)
		*root = *to_put;
	// else if (*root && (*root)->r_token && ((*root)->r_token->type == AND
	// 		|| (*root)->r_token->type == OR))
	
    // {
    //     tmp = (*root);
	// 	(*root) = *to_put;
	// 	(*root)->l_token = tmp;
	// 	// tmp = (*root)->r_token;
	// 	// (*root)->r_token = *to_put;
	// 	// (*root)->r_token->l_token = tmp;
	// }
	// else if ((*root)->type == CMD)
	else
    {
		tmp = *root;
		*root = *to_put;
		(*root)->l_token = tmp;
	}
	// else
	// 	place_pipe_node((&(*root)->r_token), to_put);
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
