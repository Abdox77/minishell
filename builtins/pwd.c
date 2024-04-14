/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:12:40 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 00:12:54 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

int	pwd(void)
{
	char	buff[PATH_MAX];

	if (getcwd(buff, sizeof(buff)) != NULL)
		printf("%s\n", buff);
	else
	{
		ft_write("minishell$ ", 2, 0);
		perror("");
		return (0);
	}
	return (1);
}
