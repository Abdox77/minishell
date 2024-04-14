/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 19:43:38 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 21:01:50 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*p1;
	unsigned char	*p2;

	i = 0;
	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	if (n > 0)
	{
		while (p1[i] && p2[i] && (p1[i] == p2[i]) && (i < n - 1))
			i++;
		if (p1[i] > p2[i])
			return (1);
		else if (p1[i] < p2[i])
			return (-1);
	}
	return (0);
}
