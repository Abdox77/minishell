/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 06:13:47 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 06:14:57 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_token *root)
{
	if (!root)
		return ;
	free_strs(root->cmd->args);
	safe_free(root->cmd->cmd_to_be_expanded);
	safe_free(root->cmd->cmd);
	free_expands(root->cmd->og_tokens);
	free_redirections(root->cmd->redir);
	safe_free(root->cmd);
	safe_free(root);
	root = NULL;
}

void	free_token(t_token *root)
{
	if (!root)
		return ;
	free_redirections(root->redir);
	free_redirections(root->og_redir);
	free(root);
}

void	cleanup(t_token *root)
{
	if (!root)
		return ;
	if (root && !root->l_token && !root->r_token)
	{
		if (root->type == CMD)
			free_cmd(root);
		else
			free_token(root);
	}
	else
	{
		cleanup(root->l_token);
		cleanup(root->r_token);
		if (root->type == CMD)
			free_cmd(root);
		else
			free_token(root);
	}
}

void	ft_error(char *error_message, int exit_code)
{
	ft_putstr_fd(error_message, STDERR_FILENO);
	exit(exit_code);
}

void	free_env_list(t_env *head)
{
	t_env	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		safe_free(tmp->key);
		safe_free(tmp->value);
		safe_free(tmp);
	}
}
