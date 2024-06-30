/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:27:24 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 23:46:46 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_and_exit(char *str)
{
	ft_write(str, 2, 1);
	exit(EXIT_FAILURE);
}

t_env	*create_node(char *key, char *value)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	memset(env, 0, sizeof(t_env));
	if (!env)
		write_and_exit("Malloc error");
	env->key = ft_strdup(key);
	if (!env->key)
		write_and_exit("Malloc error");
	if (value)
	{
		env->value = ft_strdup(value);
		if (!env->value)
			write_and_exit("Malloc error");
	}
	else
		env->value = NULL;
	env->next = NULL;
	return (env);
}

void	append_node(t_env **head, char *key, char *value)
{
	t_env	*new_node;
	t_env	*last_node;

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
