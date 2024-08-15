/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:10:30 by amohdi            #+#    #+#             */
/*   Updated: 2024/08/06 12:22:32 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl.h"

char	*ft_substrr(char *s, unsigned int start, int len)
{
	int		i;
	int		s_len;
	char	*sub;

	if (!s)
		return (NULL);
	s_len = (int)ft_strllen(s);
	if ((int)start > s_len)
		return (ft_strdupp(""));
	else if (s_len - (int)start < len)
		len = s_len - start;
	sub = (char *)malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*get_line(char *joined, char **buff)
{
	char	*line;
	size_t	len;

	free(*buff);
	if (!joined)
		return (NULL);
	len = 0;
	while (joined[len] && joined[len] != '\n')
		len++;
	if (joined[len] == '\n')
		len++;
	line = ft_substrr(joined, 0, len);
	if (!line)
		return (NULL);
	return (line);
}

char	*fix_rest(char **joined, char **r)
{
	size_t	i;
	char	*rest;

	if (!*joined)
		return (NULL);
	else if (*r)
		free(*r);
	i = 0;
	while ((*joined)[i] && (*joined)[i] != '\n')
		i++;
	if ((*joined)[i] == '\n' && (*joined)[i + 1])
	{
		rest = ft_substrr((*joined), i + 1, (ft_strllen((*joined)) - i));
		free(*joined);
		return (rest);
	}
	else
	{
		free(*joined);
		return (NULL);
	}
}

char	*get_next_line(int fd)
{
	static char	*rest;
	char		*buff;
	char		*line;
	char		*joined;
	ssize_t		bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	joined = NULL;
	buff = malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	joined = ft_sjoin(joined, rest);
	bytes = read(fd, buff, BUFFER_SIZE);
	while (bytes > 0)
	{
		buff[bytes] = '\0';
		joined = ft_sjoin(joined, buff);
		if (check_newline(buff))
			break ;
		bytes = read(fd, buff, BUFFER_SIZE);
	}
	line = get_line(joined, &buff);
	rest = fix_rest(&joined, &rest);
	return (line);
}
