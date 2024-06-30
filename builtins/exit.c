/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:14:00 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/28 22:44:50 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

int	isanumvalue(char *str)
{
	int		count;
	size_t	number;
	int		negative;

	negative = 0;
	count = 0;
	number = 0;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			negative = 1;
		str++;
	}
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		number = number * 10 + (*str++ - 48);
		count++;
	}
	if (count >= 20 || number - negative > __LONG_MAX__)
		return (0);
	return (1);
}

void	ft_exit(char **cmd)
{
	int	exit_st;
	int	i;

	exit_st = stat_handler(0, 0);
	i = 0;
	while (cmd[i])
		i++;
	ft_write("exit", 2, 1);
	if (cmd[1] && !isanumvalue(cmd[1]))
	{
		ft_write("minishell: exit: ", 2, 0);
		ft_write(cmd[1], 2, 0);
		ft_write(": numeric argument required", 2, 1);
		exit_st = 2;
	}
	else if (i > 2)
	{
		ft_write("minishell: exit: too many arguments", 2, 1);
		exit_st = 1;
	}
	else if (cmd[1] && i <= 2)
		exit_st = ft_atoi(cmd[1]);
	exit(exit_st);
}
