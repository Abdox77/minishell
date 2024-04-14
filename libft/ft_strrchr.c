/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 17:58:48 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 20:58:06 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	t;
	char	*p;
	size_t	i;
	size_t	occ;

	i = 0;
	occ = i;
	t = (char)c;
	p = (char *)s;
	while (p[i])
	{
		if (p[i] == t)
			occ = i;
		i++;
	}
	if (!p[i] && t == '\0')
		return (p + i);
	else if (p[occ] == t)
		return (p + occ);
	return (NULL);
}
