/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 19:55:39 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 21:02:54 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	x;
	unsigned char	*p;

	i = 0;
	x = (unsigned char)c;
	p = (unsigned char *)s;
	while (i < n)
	{
		if (p[i] == x)
			return ((void *)(p + i));
		i++;
	}
	return (NULL);
}
