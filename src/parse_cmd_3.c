/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:50:31 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 13:50:59 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void get_token_wout_quotes(char **line, char **arg, int *og_len)
{
    int len;
    
    len = 0;
    while ((*line)[len] && is_special_char((*line)[len]) == FALSE)
        len++;
    *arg = ft_special_join(*arg, ft_substr(*line, 0, len));
    *og_len += len;
    *line += len;
}
void get_token_wquotes(char **line, char **arg, int *og_len)
{
    char quote;
    char *buff;

    quote = **line;
    buff = get_token_in_between_quotes(line, quote, og_len);
    *arg = ft_special_join(*arg, buff);
    if(**line != quote)
        ft_print_error("Syntax error unclosed quotes\n", line, SAVE);
    else
    {
        ++(*line);
        ++(*og_len);
    }
}