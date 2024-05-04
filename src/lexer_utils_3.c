/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 09:27:47 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/04 09:34:08 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_token *handle_command(char **line)
{
    int len;
    t_token *cmd;

    cmd == NULL;
    special_trim(line);
    if(**line && is_an_operator(line) == false && **line != '|')
    {
        get_cmd(&cmd, line);
        while(**line)
        {
            len = 0;
            special_trim(line);
        }
    }
    return cmd;
}