/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 21:57:10 by amohdi            #+#    #+#             */
/*   Updated: 2024/07/02 22:22:13 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	args_len(char **args)
{
	int	i;

	i = 0;
	if (!args || !*args)
	{
		return (0);
	}
	while (args[i])
		i++;
	return (i);
}

void	set_envvalue(t_env *env, char **args)
{
	t_env	*tmp;
	t_env	*new_node;
	int		len;
	char	*val;

	len = args_len(args);
	if (!len)
		return ;
	val = args[len - 1];
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "_") == 0)
			return (free(tmp->value), tmp->value = ft_strdup(val), (void)0);
		tmp = tmp->next;
	}
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (perror("malloc"), (void)0);
	new_node->key = ft_strdup("_");
	new_node->value = ft_strdup(val);
	new_node->next = env->next;
	env->next = new_node;
}
