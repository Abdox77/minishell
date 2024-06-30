/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 05:08:04 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 05:16:29 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *find_env_value(const char *var_name, t_env *env_list)
{
    while (env_list != NULL)
    {
        if (strcmp(var_name, env_list->key) == 0)
            return (env_list->value);
        env_list = env_list->next;
    }
    return (NULL);
}

size_t get_var_length(const char **str_ptr, t_env *env_list)
{
    const char  *var_start;
    size_t      var_len;
    char        *var_name;
    char        *var_value;

    (*str_ptr)++;
    var_start = *str_ptr;
    while (**str_ptr && (ft_isalnum(**str_ptr) || **str_ptr == '_'))
        (*str_ptr)++;
    var_len = *str_ptr - var_start;
    var_name = ft_strndup(var_start, var_len);
    var_value = find_env_value(var_name, env_list);
    free(var_name);
    if (var_value)
        return (strlen(var_value));
    return (0);
}

size_t get_quoted_length(const char **str_ptr, t_env *env_list)
{
    size_t length = 0;

    (*str_ptr)++;
    while (**str_ptr && **str_ptr != '"')
    {
        if (**str_ptr == '$')
            length += get_var_length(str_ptr, env_list);
        else
        {
            length++;
            (*str_ptr)++;
        }
    }
    if (**str_ptr == '"')
        (*str_ptr)++;
    return (length);
}

void append_quoted_value(char **res_ptr, const char **str_ptr, t_env *env_list)
{
    (*str_ptr)++;
    while (**str_ptr && **str_ptr != '"')
    {
        if (**str_ptr == '$')
            append_var_value(res_ptr, str_ptr, env_list);
        else
        {
            **res_ptr = **str_ptr;
            (*res_ptr)++;
            (*str_ptr)++;
        }
    }
    if (**str_ptr == '"')
        (*str_ptr)++;
}

int check_to_expand(char *cmd, t_env *env_list)
{
    int i = 0;

    (void)env_list;
    if (!cmd || cmd[i] != '$')
        return (1);
    if (is_quoted(cmd))
        return (1);
    i++; // Skip the initial $
    if (!is_expandable_char(cmd[i]))
        return ( 1);
    while (cmd[i]) {
        if (!is_expandable_char(cmd[i]))
            return ( 1);
        cmd++;
    }
    char *xp = expand_string1(cmd, env_list);
    if (xp)
        return (free(xp), 1);
    return (free(xp), 0);
}