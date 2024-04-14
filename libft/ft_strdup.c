/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 21:50:59 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 14:04:31 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	srclen;
	size_t	i;
	char	*p;

	i = 0;
	srclen = ft_strlen(s1);
	p = (char *)malloc(sizeof(char) * srclen + 1);
	if (!p)
		return (NULL);
	while (i < srclen)
	{
		p[i] = s1[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
