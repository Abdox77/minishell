/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:47:35 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 13:48:05 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool arg_len_condition(char **line, int len)
{
    if ((*line)[len] && (*line)[len] != '|' && is_space((*line)[len]) == FALSE && is_redirection_char((*line)[len]) == FALSE && 
        is_an_operator(line , len) == FALSE && is_parenthesis((*line)[len]) == FALSE)
    {
        return (TRUE);
    }
    return FALSE;
}
t_bool process_line_condition(char **line)
{
    if (**line != '\0' && is_an_operator(line, 0) == FALSE && **line != '|')
        return TRUE;
    return FALSE;
}

t_bool     handle_parenthesis_error(char **line, int len)
{
    if ((*line)[len] == '(')
        return(ft_print_error("Syntax Error unexpected token '('\n", line, SAVE), TRUE);
    return FALSE;
}
t_bool check_for_quotes_condition(char **line , t_lvars *vars)
{
    if ((*line)[vars->len] && (*line)[vars->len] != '|' && is_space((*line)[vars->len]) == FALSE 
        && is_redirection_char((*line)[vars->len]) == FALSE && is_an_operator(line , vars->len) == FALSE 
        && is_parenthesis((*line)[vars->len]) == FALSE)
    {
        return TRUE;
    }
    return FALSE;
}

t_bool check_for_quotes(char **line, t_lvars *vars)
{
    while(check_for_quotes_condition(line , vars) == TRUE)
    {
        if (is_quote((*line)[vars->len]) == TRUE)
            return TRUE;
        vars->len++;
    }
    return FALSE;
}