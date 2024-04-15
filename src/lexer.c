/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 15:25:20 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void lexer(t_token *token, char **line)
{
    if (!line || !*line || !**line)
        return;
    if (**line == '<')
        handle_redirection(token, line, INFILE);
    else if (**line == '>')
        handle_redirection(token, line, OUTFILE);
    // else if (*line == '|')
    //     handle_pipe(token, line);
    else
        handle_command(token, line);
    lexer(token, line);
}
*/


void manage_command(t_token *token, char **line)
{
    if (is_redirection(**line) == true)
        handle_redirection(token, line);
    else if (**line == '|')
        return ;
    else
        handle_command(token, line);
}

void handle_pipe(t_token *token, char **line)
{
    if (token)
        ft_error("wth token is not null", EXIT_FAILURE);
    token = set_pipe_head();
    if (!token)
        ft_error("failed to create token", EXIT_FAILURE;
    ++(*line);
}


void lexer(t_token *token, char **line)
{
    if (!line || !*line || !**line)
        return;
    if (**line == '|')
        handle_pipe(token->r_token, line);
    else
        manage_command(token->l_token, line);
    lexer(token, line);
}
