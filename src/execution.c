/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 05:19:28 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 05:36:10 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(t_token *token, t_exec *exec)
{
	int	status;

	status = 0;
	if (!token)
		return (0);
	if (token->redir)
		execute_subtree(token, exec);
	else if (token->type == PIPE)
		execute_pipe(token, exec);
	else if (token->type == AND)
		execute_and(token, exec);
	else if (token->type == OR)
		execute_or(token, exec);
	else if (token->type == CMD)
		status = execute_command(token, exec);
	return (status);
}

void	execute_and(t_token *node, t_exec *exec)
{
	execute(node->l_token, exec);
	if (stat_handler(0, 0) == 0)
		execute(node->r_token, exec);
}

void	execute_or(t_token *node, t_exec *exec)
{
	execute(node->l_token, exec);
	if (stat_handler(0, 0) != 0)
		execute(node->r_token, exec);
}

void	reset_fd(int in, int out)
{
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
}
