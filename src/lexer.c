/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/17 18:37:55 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void manage_command(t_token **token, char **line)
{
    if (!*token)
        printf("not token in manage commands\n");
    if (*line && **line == '|')
    {
        ++(*line);
        while(*line && is_space(**line) == true)
            ++(*line);
        (*token)->r_token = set_pipe_head();
        if (!(*token)->r_token)
            printf("failed to create head of token\n");
        lexer(&((*token)->r_token), line);
    }
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
        handle_pipe(token, line);
    else
        manage_command(&((*token)->l_token), line);
    /*if (*line)
        lexer(token, line);
*/
}
