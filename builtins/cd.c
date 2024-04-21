/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:11:48 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/21 06:30:10 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

static int	check_key(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

char	*get_value(t_env *env, char *key)
{
	t_env	*tmp;

	if (!env || !key)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int cd(char **arg, t_exec *exec)
{
    int	i;

	i = 1;
	if (!arg[i])
	{
		if (!check_key(exec->env, "HOME"))
		{
			perror(GREEN"minishell$ " "\033[35m");
			return (0);
		}
		else
		{
			if (chdir(get_value(exec->env, "HOME")) == -1)
			{
				(perror(GREEN"minishell$ " "\033[35m"));
				return (0);
			}
		}
	}
	else if (chdir(arg[1]) == -1)
	{
		(perror(GREEN"minishell$ " "\033[35m"));
		return (0);
	}
	return (1);
}
