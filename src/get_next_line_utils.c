/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:46:07 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/03/07 16:48:48 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	get_len(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

static char	*ft_strdup(char *str)
{
	char	*ptr;
	size_t	len;
	size_t	i;

	len = get_len(str);
	ptr = malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_join(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*joined;

	if (!s2)
		return (NULL);
	if (!s1)
	{
		joined = ft_strdup(s2);
		return (joined);
	}
	i = 0;
	j = 0;
	joined = malloc(sizeof(char) * (get_len(s1) + get_len(s2) + 1));
	if (!joined)
		return (free(s1), NULL);
	while (s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	while (s2[j])
		joined[i++] = s2[j++];
	joined[i] = '\0';
	return (joined);
}

int	n_search(char *s)
{
	int	i;

	i = 0;
	while (i <= get_len(s))
	{
		if (s[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}
