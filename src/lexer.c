/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/16 21:21:51 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void manage_command(t_token **token, char **line)
{
    if (is_redirection_char(**line) == true)
        handle_redirection(token, line);
    else if (**line != '|')
        handle_command(token, line);
}

void handle_pipe(t_token **token, char **line)
{
    *token = set_pipe_head();
    if (!*token)
        ft_error("failed to create token", EXIT_FAILURE);
    ++(*line);
    lexer(&((*token)->r_token), line);
}

void lexer(t_token **token, char **line)
{
    if (!line || !*line || !**line)
        return;
    if (**line == '|')
        handle_pipe(&((*token)->r_token), line);
    else
        manage_command(&((*token)->l_token), line);
    if (*line)
        lexer(token, line);
}
