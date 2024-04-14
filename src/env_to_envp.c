/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 20:08:31 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/04/14 20:18:39 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_envp(t_exec *exec)
{
	t_env	*tmp;
	int		i;
	char	**arr;

    i = 0;
	tmp = exec->env;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	tmp = exec->env;
	arr = malloc((sizeof(char *) * i + 1));
	if (!arr)	
		return (NULL);
	i = 0;
	while (tmp)
	{
		arr[i] = ft_strjoin(tmp->key, "=");
		arr[i] = ft_strjoin(arr[i], tmp->value);
		tmp = tmp->next;
		i++;
	}
	// arr[i] = NULL;
	return (arr);
}
