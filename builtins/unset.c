/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:13:26 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 15:24:25 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

static void free_env_node(t_env **env, char *arg)
{
    t_env   *tmp;
    t_env   *prev;

    prev = NULL;
    tmp = *env;
    while (tmp)
    {
        if (!ft_strncmp(tmp->key, arg, ft_strlen(tmp->key)))
        {
            if (prev == NULL)
                *env = tmp->next;
            else
                prev->next = tmp->next;
            free(tmp->key);
            free(tmp->value);
            free(tmp);
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

int unset(t_env **env, char **cmd)
{
    int     i;
    int     j;

    i = 1;
    while (cmd[i])
    {
        j = 0;
        while (cmd[i][j])
        {
            if (!ft_isalnum(cmd[i][j]))
            {
                ft_write("unset: `", 2, 0);
                ft_write(cmd[i], 2, 0);
                ft_write("` invalid parameter name\n", 2, 0);
                return 0;
            }
            j++;
        }
        free_env_node(env, cmd[i]);
        i++;
    }
    return 1;
}
