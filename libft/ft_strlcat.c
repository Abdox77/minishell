/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:41:45 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 14:04:53 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	slen;
	size_t	dlen;

	if (!dst && !size && src)
		return (ft_strlen(src));
	i = 0;
	slen = ft_strlen(src);
	dlen = ft_strlen(dst);
	if (size <= dlen)
		return (size + slen);
	if (size > 0)
	{
		while ((dlen + i < size - 1) && src[i])
		{
			dst[dlen + i] = src[i];
			i++;
		}
		dst[dlen + i] = '\0';
	}
	return (dlen + slen);
}
