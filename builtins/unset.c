/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:13:26 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/09 18:05:25 by aabou-ib         ###   ########.fr       */
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
            if (!ft_isalnum(cmd[i][j]) && cmd[i][j] != 95)
            {
                ft_write("unset: `", 2, 0);
                ft_write(cmd[i], 2, 0);
                ft_write("` invalid parameter name\n", 2, 0);
                stat(1,1);
                // exit(1);
                return 0;
            }
            j++;
        }
        free_env_node(env, cmd[i]);
        i++;
    }
    return 1;
}
