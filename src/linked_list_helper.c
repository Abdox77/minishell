/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:27:24 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/04/16 21:04:43 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env   *create_node(char *key, char *value)
{
    t_env   *env;

    env = (t_env *)malloc(sizeof(t_env));
    if (!env)
    {
        ft_write("Malloc error", 2, 1);
        exit(2);
    }
    env->key = ft_strdup(key);
    if (!env->key)
    {
        ft_write("Malloc error", 2, 1);
        exit(3);
    }
    if (value) {
        env->value = ft_strdup(value);
        if (!env->value)
        {
            ft_write("Malloc error", 2, 1);
            exit(4);
        }
    } 
    else
        env->value = NULL;
    env->next = NULL;
    return (env);
}

void    append_node(t_env **head, char *key, char *value)
{
    t_env    *new_node;
    t_env    *last_node;

    new_node = create_node(key, value);
    if (!head || !(*head))
    {
        (*head) = new_node;
        return ;
    }
    last_node = (*head);
    while (last_node->next != NULL)
        last_node = last_node->next;
    last_node->next = new_node;
}