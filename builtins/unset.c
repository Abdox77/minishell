/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:13:26 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/27 18:51:39 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

static void	free_env_node(t_env **env, char *arg)
{
	t_env	*tmp;
	t_env	*prev;

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
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	unset(t_env **env, char **cmd)
{
	int	i;

	i = 1;
	while (cmd[i])
	{
		free_env_node(env, cmd[i]);
		i++;
	}
	stat_handler(0, 1);
	return (1);
}
