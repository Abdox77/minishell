/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 17:17:49 by amohdi           ###   ########.fr       */
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
    printf("got here in handle_pipe\n");
    if (*token)
        ft_error("wth token is not null", EXIT_FAILURE);
    (*token)->r_token = set_pipe_head();
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
        handle_pipe(token, line);
    else
        manage_command(token, line);
    if (*line)
        lexer(token, line);
}
