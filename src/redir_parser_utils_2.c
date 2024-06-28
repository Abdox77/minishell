/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 20:46:17 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/28 20:53:10 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

REDIR_MODE get_redir_mode(char **line)
{
    REDIR_MODE mode;

    if (**line == '<')
    {
        mode = INFILE;
        ++(*line);
        if (**line == '<')
        {
            ++(*line);
            mode = HEREDOC;
        }
    }
    else
    {
        mode = TRUNC;
        ++(*line);
        if (**line == '>')
        {
            ++(*line);
            mode = APPEND;
        }
    }
    special_trim(line);
    return mode;
}