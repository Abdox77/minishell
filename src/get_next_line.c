/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 03:18:41 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 13:12:19 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	join_and_free(char **buffer, char *readed)
{
	char	*temp;

	temp = ft_join(*buffer, readed);
	free(*buffer);
	*buffer = temp;
}

static char	*read_from_line(char *buffer, int fd)
{
	int		r;
	char	*readed;

	r = 1;
	readed = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!readed)
		return (NULL);
	while (r > 0)
	{
		r = read(fd, readed, BUFFER_SIZE);
		if (r == 0)
			break ;
		if (r == -1)
		{
			free(buffer);
			free(readed);
			return (NULL);
		}
		readed[r] = '\0';
		join_and_free(&buffer, readed);
		if (n_search(readed))
			break ;
	}
	return (free(readed), buffer);
}

static char	*add_to_line(char *buffer)
{
	int		i;
	char	*line;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
	{
		line[i] = '\0';
		i++;
	}
	else
		line[i] = '\0';
	return (line);
}

static char	*delete_newline(char *buffer)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	j = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	new = malloc(sizeof(char) * (get_len(buffer) - i + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	while (buffer[i])
		new[j++] = buffer[i++];
	new[j] = '\0';
	return (free(buffer), new);
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	char		*line;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX)
		return (NULL);
	buffer = read_from_line(buffer, fd);
	if (!buffer)
		return (NULL);
	line = add_to_line(buffer);
	buffer = delete_newline(buffer);
	if (line[0] == '\0' && buffer[0] == '\0')
	{
		free(line);
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	return (line);
}
