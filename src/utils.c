/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 09:13:22 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/12 13:02:21 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int strs_len(char **args)
{
    int i;

    i = 0;
    if (!args)
        return (0);
    while(args[i])
        i++;
    return (i);
}


int	ft_strcmp(const char *s1, const char *s2)
{
    int i;
	unsigned char	*p1;
	unsigned char	*p2;

    i = 0;
	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (p1[i] && p2[i] && (p1[i] == p2[i]))
		i++;
	if (p1[i] > p2[i])
		return (1);
	else if (p1[i] < p2[i])
		return (-1);
	return (0);
}

char	*ft_tolowerr(char *c)
{
	int	i;

	i = 0;
	while (c[i])
	{
		if (c[i] >= 'A' && c[i] <= 'Z')
			c[i] += 32;
		i++;
	}
	return (c);
}