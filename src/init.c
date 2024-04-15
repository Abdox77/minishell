/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 01:45:18 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 11:49:53 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *new_token(TOKEN type)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    memset(token, 0, sizeof(t_token));
    token->type = type;
    if (type== CMD)
        token->cmd = new_cmd();
    token->value = NULL;
    token->l_token = NULL;
    token->r_token = NULL;
    return (token);
}


t_redir *new_cmd_redir (REDIR_MODE mode, char *file_name)
{
    t_redir *new;

    new = malloc (sizeof(t_redir));
    if (!new)
        ft_error("not new redir\n", EXIT_FAILURE);
    new->mode = mode;
    new->file_name = file_name;
    new->next = NULL;
    return (new);
}

t_token     *set_pipe_head(void)
{
    t_token *head;

    head = new_token(PIPE);
    if (!head)
        return (NULL);
    head->type = PIPE;
    return (head);
}
