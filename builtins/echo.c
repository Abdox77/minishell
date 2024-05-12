/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:11:05 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/12 13:54:48 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

static int	n_exists(char *arg)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(char **arg)
{
	int	i;
	// int	j;
	int	flag;

	i = 1;
	flag = 0;
	if (!arg[i])
	{
		ft_write("\n", 1, 0);
		return (1);
	}
	while (arg[i])
	{
		if (arg[i][0] == '-')
		{
			if (arg[i][1] == 'n' && n_exists(arg[i]))
			{
				flag = 1;
				i++;
				continue ;
			}
		}
		// if (arg[i][0] == '$' && arg[i][1] == '?')
		// {
        //     //need to do implementation for last exit status
		// }
		if (arg[i])
		{
			ft_write(arg[i], 1, 0);
			if (arg[i] && arg[i + 1])
				ft_write(" ", 1, 0);
		}
		i++;
	}
	if (flag == 0)
		ft_write("\n", 1, 0);
	return (0);
}