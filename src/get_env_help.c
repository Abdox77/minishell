/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 23:39:19 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 23:40:11 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_write(char *str, int fd, int new_line)
{
	while (*str)
	{
		write(fd, str, 1);
		str++;
	}
	if (new_line == 1)
		write(fd, "\n", 1);
}

void	copy_key(char **env, t_var *var)
{
	var->tmp_key = ft_substr(env[var->i], 0, var->j);
	if (!var->tmp_key)
	{
		ft_write("Malloc error", 2, 1);
		exit(2);
	}
	var->rem = var->j + 1;
}
