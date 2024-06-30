/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_var_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 05:17:54 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 05:18:53 by aabou-ib         ###   ########.fr       */
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

void append_var_value(char **res_ptr, const char **str_ptr, t_env *env)
{
    const char  *var_start;
    size_t      var_len;
    char        *var_name;
    char        *var_value;

    (*str_ptr)++;
    var_start = *str_ptr;
    while (**str_ptr && (isalnum(**str_ptr) || **str_ptr == '_'))
        (*str_ptr)++;
    var_len = *str_ptr - var_start;
    var_name = strndup(var_start, var_len);
    var_value = find_env_value(var_name, env);
    free(var_name);
    if (var_value)
    {
        ft_strcpy(*res_ptr, var_value);
        *res_ptr += strlen(var_value);
    }
}
