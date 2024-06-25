/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:12:40 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/25 01:26:34 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

int	pwd(void)
{
	char	buff[PATH_MAX];

	if (getcwd(buff, sizeof(buff)) != NULL)
	{
		stat(0, 1);
		printf("%s\n", buff);
	}
	else
	{
		ft_write("minishell$ ", 2, 0);
		perror("");
		stat(1, 1);
		return (0);
	}
	return (1);
}
