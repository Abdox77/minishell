/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:02:13 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 05:44:03 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_quote_in_expanded_line(t_exec *exec, char *line, char **expanded_line, t_iter_vars *vars)
{
    char *buff;

    buff = NULL;
    *expanded_line = ft_strjoin(*expanded_line,
                        ft_substr(line, vars->i, vars->len + 1));
    vars->i += vars->len + 1;
    buff = get_value_in_between_quotes(line + vars->i, line[vars->i - 1]);
    if (buff)
        vars->i += ft_strlen(buff);
    buff = expand_in_heredoc(exec, buff);
    *expanded_line = ft_strjoin(*expanded_line , buff);
}

static void handle_consecutive_dollarsign(char *line, char **expanded_line, t_iter_vars *vars)
{
    if (line[vars->i + 1] == '$')
    {
        *expanded_line = ft_strjoin(*expanded_line, ft_substr(line, vars->i, 2));
        vars->i += 2;
    }
    else 
    {
        *expanded_line = ft_strjoin(*expanded_line, ft_substr(line, vars->i, 1));
        ++(vars->i);
    }
}

static t_bool is_valid_consecutive_dollarsign(char *s)
{
    if (*s == '$' && (*(s + 1) == '$'|| (ft_isalpha(*(s + 1)) == 0 && *(s + 1) != '_')))
        return (TRUE);
    return FALSE;
}

static void handle_single_dollarsign(t_exec *exec, char *line, char **expanded_line, t_iter_vars *vars)
{
    char *buff;
    char *env_variable;

    buff = NULL;
    env_variable = NULL;
    vars->len = 0;
    (vars->i)++;
    while(is_valid_char_for_expansion(line[vars->i + vars->len]) == TRUE && is_space(line[vars->i + vars->len]) == FALSE)
        ++(vars->len);
    if (vars->len)
    {
        buff = ft_substr(line, vars->i, vars->len);
        env_variable = ft_get_value(exec, buff);
        *expanded_line = ft_strjoin(*expanded_line, env_variable);
        vars->i += vars->len;
    }
}

char *expand_in_heredoc(t_exec *exec, char *line)
{
    char *expanded_line;
    t_iter_vars vars;

    if (!line)
        return NULL;
    init_iter_vars(&vars);
    expanded_line = NULL;
    while(line[vars.i])
    {
        vars.len = 0;
        while (is_valid_char_for_expansion(line[vars.i + vars.len]) == TRUE)
            vars.len++;
        if (is_quote(line[vars.i + vars.len]) == TRUE)
            handle_quote_in_expanded_line(exec, line, &expanded_line, &vars);
        else if (vars.len)
        {
            expanded_line = ft_strjoin(expanded_line, ft_substr(line, vars.i, vars.len));
            vars.i += vars.len;
        }
        else if (is_valid_consecutive_dollarsign(&(line[vars.i])) == TRUE)
            handle_consecutive_dollarsign(line, &expanded_line, &vars);    
        else if (line[vars.i] == '$')
            handle_single_dollarsign(exec, line, &expanded_line, &vars);
    }
    return (expanded_line);
}
