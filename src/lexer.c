/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/21 06:56:35 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void manage_command(t_token **token, char **line)
// {
//     if (!*token)
//         handle_command(token, line);
//     if (*line && **line == '|')
//     {
//         ++(*line);
//         while(*line && is_space(**line) == true)
//             ++(*line);
//         (*token)->r_token = set_pipe_head();
//         if (!(*token)->r_token)
//             printf("failed to create head of token\n");
//         lexer(&((*token)->r_token), line);
//     }
// }

void handle_pipe(t_token **token, char **line)
{
    if (*token && (*token)->r_token)
        printf("token is not null\n");
    (*token)->r_token = set_pipe_head();
    if (!*token)
        ft_error("failed to create token", EXIT_FAILURE);
    ++(*line);
    printf("line is in handle pipe%s\n", *line);
    lexer(&((*token)->r_token), line);
}

void lexer(t_token **token, char **line)
{
    if (!line || !*line || !**line)
        return;
    if (**line == '|')
        handle_pipe(token, line);
    else
        handle_command(&((*token)->l_token), line);
    if (*line)
        lexer(token, line);
}
