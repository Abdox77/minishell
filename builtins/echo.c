/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:11:05 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/25 01:07:12 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

	// static int	n_exists(char *arg)
	// {
	// 	int	i;

	// 	i = 1;
	// 	while (arg[i])
	// 	{
	// 		if (arg[i] != 'n')
	// 			return (0);
	// 		i++;
	// 	}
	// 	return (1);
	// }

	// int	ft_echo(char **arg)
	// {
	// 	int	i;
	// 	int	flag;

	// 	i = 1;
	// 	flag = 0;
	// 	if (!arg)
	// 	{
	// 		ft_write("\n", 1, 0);
	// 		return (1);
	// 	}
	// 	while (arg[i])
	// 	{
	// 		if (arg[i][0] == '-')
	// 		{
	// 			if (arg[i][1] == 'n' && n_exists(arg[i]))
	// 			{
	// 				flag = 1;
	// 				i++;
	// 				continue ;
	// 			}
	// 		}
	// 		if (arg[i])
	// 		{
	// 			printf("%s",arg[i]);
	// 			if (arg[i] && arg[i + 1])
	// 				printf(" ");
	// 		}
	// 		i++;
	// 	}
	// 	if (flag == 0)
	// 		printf("\n");
	// 	stat(0, 1);
	// 	return (0);
	// }

static int n_exists(char *arg)
{
    int i;

    i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

static int handle_flag(char **arg, int *i)
{
    int flag;

    flag = 0;
    if (arg[*i] && arg[*i][0] == '-' && arg[*i][1] == 'n' && n_exists(arg[*i]))
    {
        flag = 1;
        (*i)++;
    }
    return (flag);
}

int ft_echo(char **arg)
{
    int i;
    int flag;

    i = 1;
	stat(0, 1);
    if (!arg || !arg[1])
    {
        ft_write("\n", 1, 0);
        return (1);
    }
    flag = handle_flag(arg, &i);
    while (arg[i])
    {
        printf("%s", arg[i]);
        if (arg[i + 1])
            printf(" ");
        i++;
    }
    if (!flag)
        printf("\n");
    return (0);
}
