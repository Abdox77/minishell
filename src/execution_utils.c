/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 02:25:05 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/02 18:12:15 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

void	free_2d(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*get_cmd(char *cmd, char **envp)
{
	char	**tab;
	char	*pathcmd;
	char	*tmp_pathcmd;
	int		j;

	j = 0;
	if (ft_strchr(cmd, '/'))
	{
		return (ft_strdup(cmd));
		return (NULL);
	}
	tab = find_path(envp);
	if (!tab)
		return (NULL);
	while (tab[j] && tab)
	{
		tmp_pathcmd = ft_strjoin("/", cmd);
		pathcmd = ft_strjoin(tab[j], tmp_pathcmd);
		free(tmp_pathcmd);
		if (access(pathcmd, F_OK | X_OK) != -1)
			return (free_strs(tab), pathcmd);
		free(pathcmd);
		j++;
	}
	return (free_strs(tab), NULL);
}

void	*malloc_with_error(size_t size)
{
	void	*ptr;

	ptr = malloc(size + 2);
	if (ptr == NULL)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}
