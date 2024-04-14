/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 21:16:36 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 20:58:43 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!haystack && !len)
		return (NULL);
	if (needle[0] == '\0')
		return ((char *)(haystack));
	while (haystack[i] && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && haystack[i + j] && needle[j] && i
			+ j < len)
		{
			if (!needle[j + 1])
				return ((char *)(haystack + i));
			j++;
		}
		i++;
	}
	return (NULL);
}
