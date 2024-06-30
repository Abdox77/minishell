/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subtree.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 05:35:51 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 13:19:08 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_and_sub(t_token *node, t_exec *exec)
{
	execute_subtree(node->l_token, exec);
	if (stat_handler(0, 0) == 0)
		execute_subtree(node->r_token, exec);
}

void	execute_or_sub(t_token *node, t_exec *exec)
{
	execute_subtree(node->l_token, exec);
	if (stat_handler(0, 0) != 0)
		execute_subtree(node->r_token, exec);
}

void	dup_and_close(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}

void	execute_subtree(t_token *root, t_exec *exec)
{
	int	original_stdin;
	int	original_stdout;

	if (!root)
		return ;
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (root)
		handle_redirections(root->redir, root->og_redir, exec->env, exec);
	if (root->type == CMD)
		execute_command(root, exec);
	else if (root->type == PIPE)
		execute_pipe(root, exec);
	else if (root->type == AND)
	{
		execute_and_sub(root, exec);
	}
	else if (root->type == OR)
	{
		execute_or_sub(root, exec);
	}
	dup_and_close(original_stdin, original_stdout);
}
