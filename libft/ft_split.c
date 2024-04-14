/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:56:01 by amohdi            #+#    #+#             */
/*   Updated: 2023/11/06 20:50:48 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_delim(char c, char d)
{
	if (c == d)
		return (1);
	return (0);
}

static int	ft_count_seg(char const *s, char c)
{
	int		count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (!ft_is_delim(s[i], c))
		{
			while (s[i] && !ft_is_delim(s[i], c))
				i++;
			count++;
		}
		if (s[i])
			i++;
	}
	return (count);
}

static size_t	ft_seg_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && !ft_is_delim(s[len], c))
		len++;
	return (len);
}

static char	*ft_seg_dup(char const *s, char c)
{
	size_t	i;
	size_t	len;
	char	*seg;

	i = 0;
	len = ft_seg_len(s, c);
	seg = (char *)malloc(sizeof(char) * len + 1);
	if (!seg)
		return (NULL);
	while (i < len)
	{
		seg[i] = s[i];
		i++;
	}
	seg[i] = '\0';
	return (seg);
}

char	**ft_split(char const *s, char c)
{
	int		j;
	size_t	i;
	int		count;
	char	**splitted;

	if (!s)
		return (NULL);
	i = 0;
	j = 0;
	count = ft_count_seg(s, c);
	splitted = (char **)malloc(sizeof(char *) * (count + 1));
	if (!splitted)
		return (NULL);
	while (s[i])
	{
		if (!ft_is_delim(s[i], c))
		{
			splitted[j++] = ft_seg_dup(&s[i], c);
			i += ft_seg_len(&s[i], c);
		}
		else
			i++;
	}
	splitted[j] = NULL;
	return (splitted);
}
