/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:11:12 by amohdi            #+#    #+#             */
/*   Updated: 2024/08/03 07:53:04 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl.h"

int	ft_strllen(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	check_newline(char *buff)
{
	int	i;

	if (!buff)
		return (0);
	i = 0;
	while (buff[i])
	{
		if (buff[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strdupp(char *s1)
{
	int	srclen;
	int	i;
	char	*p;

	i = 0;
	srclen = ft_strllen(s1);
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

char	*ft_j_strcat(char *s1, const char *p, int len)
{
	int	i;
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

char	*ft_sjoin(char *s1, char *s2)
{
	char	*p;
	int	i;
	int	l1;
	int	l2;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		return (ft_strdupp(s2));
	else if (!s2 && s1)
		return (ft_strdupp(s1));
	i = 0;
	l1 = ft_strllen((char *)s1);
	l2 = ft_strllen((char *)s2);
	p = (char *)malloc(sizeof(char) * (l1 + l2 + 1));
	if (!p)
		return (NULL);
	while (i < l1)
	{
		p[i] = ((char *)s1)[i];
		i++;
	}
	ft_j_strcat((p + i), s2, l2);
	free(s1);
	return (p);
}
