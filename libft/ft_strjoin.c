/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:11:05 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 20:54:38 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_j_strcat(char *s1, const char *p, size_t len)
{
	size_t	i;
	char	*s2;

	i = 0;
	s2 = (char *)p;
	while (i < len)
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*p;
	size_t	i;
	size_t	l1;
	size_t	l2;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		return (ft_strdup(s2));
	else if (!s2 && s1)
		return (ft_strdup(s1));
	i = 0;
	l1 = ft_strlen((char *)s1);
	l2 = ft_strlen((char *)s2);
	p = (char *)malloc(sizeof(char) * (l1 + l2 + 1));
	if (!p)
		return (NULL);
	while (i < l1)
	{
		p[i] = ((char *)s1)[i];
		i++;
	}
	ft_j_strcat((p + i), s2, l2);
	return (p);
}
