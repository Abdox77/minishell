/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 01:45:18 by amohdi            #+#    #+#             */
/*   Updated: 2024/07/02 13:17:22 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(enum e_token type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	memset(token, 0, sizeof(t_token));
	token->type = type;
	token->was_in_parenthesis = FALSE;
	if (type == CMD)
	{
		token->cmd = new_cmd();
		token->cmd->og_tokens = malloc(sizeof(t_expands));
		memset(token->cmd->og_tokens, 0, sizeof(t_expands));
		if (!token->cmd)
			printf("failed to create new cmd\n");
	}
	return (token);
}

t_redir	*new_cmd_redir(enum e_redir_mode mode, char *file_name)
{
	t_redir	*new;

	if (!file_name)
		ft_print_error("Syntax error near unexpected token 'new line'\n", NULL,
			SAVE);
	new = malloc(sizeof(t_redir));
	if (!new)
		printf("not new redir\n");
	new->mode = mode;
	new->to_be_expanded = FALSE;
	if (file_name)
	{
		new->file_name = ft_strdup(file_name);
		free(file_name);
	}
	else
		new->file_name = NULL;
	new->next = NULL;
	return (new);
}

t_token	*set_pipe_head(void)
{
	t_token	*head;

	head = new_token(PIPE);
	if (!head)
		return (NULL);
	memset(head, 0, sizeof(t_token));
	head->type = PIPE;
	return (head);
}
