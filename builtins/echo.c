/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:11:05 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/18 21:08:14 by amohdi           ###   ########.fr       */
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

	// stat(0, 1);
	i = 1;
	flag = 0;
	if (!arg)
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
		if (arg[i])
		{
			printf("%s",arg[i]);
			if (arg[i] && arg[i + 1])
				printf(" ");
		}
		i++;
	}
	if (flag == 0)
		printf("\n");
	stat(0, 1);
	// exit(0);
	return (0);
}