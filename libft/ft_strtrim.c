/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:18:31 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 20:49:03 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_in_set(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_trim(size_t len, char *s, char const *set)
{
	char	*trim;
	size_t	i;

	i = 0;
	while (ft_is_in_set(s[len], set))
		len--;
	len++;
	trim = (char *)malloc(sizeof(char) * len + 1);
	if (!trim)
		return (NULL);
	while (i < len)
	{
		trim[i] = s[i];
		i++;
	}
	trim[i] = '\0';
	return (trim);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s;
	size_t	len;

	s = (char *)s1;
	if (!s)
		return (NULL);
	if (!set)
		return (ft_strdup(s));
	while (*s && ft_is_in_set(*s, set))
		s++;
	if (!s[0])
		return (ft_strdup(s));
	len = ft_strlen(s) - 1;
	return (ft_trim(len, s, set));
}
