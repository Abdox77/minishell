/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:34:53 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 14:00:35 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*s1;
	unsigned char	*s2;

	if (!dst && !src)
		return (NULL);
	i = 0;
	s1 = (unsigned char *)dst;
	s2 = (unsigned char *)src;
	if (s1 > s2)
	{
		while (len-- > 0)
			s1[len] = s2[len];
	}
	else
	{
		while (i < len)
		{
			s1[i] = s2[i];
			i++;
		}
	}
	return ((void *)s1);
}
