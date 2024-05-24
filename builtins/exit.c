/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:14:00 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 15:24:45 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

#include "../src/minishell.h"

void	ft_exit(char **cmd)
{
	int	exit_st;
	int	i;

	exit_st = stat(0,0);
	i = 0;
	while (cmd[i])
		i++;
	ft_write("exit", 1, 1);
	if (i > 2)
	{
		ft_write("minishell: exit: too many arguments", 2, 1);
		exit_st = 1;
	}
	if (cmd[1] && i <= 2)
		exit_st = ft_atoi(cmd[1]);
	exit(exit_st);
}