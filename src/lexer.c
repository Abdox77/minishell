/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 11:42:52 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
