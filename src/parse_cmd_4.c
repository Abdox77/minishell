/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:52:34 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 18:22:08 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_arg(char **args, char *arg)
{
	int		i;
	int		len;
	char	**new_args;

	i = -1;
	len = strs_len(args);
	new_args = malloc(sizeof(char *) * (len + 2));
	while (args[++i])
	{
		new_args[i] = ft_strdup(args[i]);
		free(args[i]);
	}
	new_args[i] = ft_strdup(arg);
	free(arg);
	free(args);
	new_args[i + 1] = NULL;
	return (new_args);
}

char	*copy_token_in_between_quotes(char **line, int len, int *og_len)
{
	int		i;
	char	*token;

	i = -1;
	token = malloc(sizeof(char) * len + 1);
	if (!token)
		return (NULL);
	while (++i < len)
		token[i] = (*line)[i];
	token[i] = '\0';
	*line += len;
	*og_len += len;
	return (token);
}

char	*get_token_in_between_quotes(char **line, char quote, int *og_len)
{
	int		len;
	char	*token;

	len = 0;
	if (!*line || !**line)
		return (NULL);
	++(*line);
	++(*og_len);
	while ((*line)[len] && (*line)[len] != quote)
		len++;
	if (!(*line)[len])
	{
		if (0 == len && (*line)[len] == quote)
		{
			++(*line);
			++(*og_len);
		}
		return (NULL);
	}
	token = copy_token_in_between_quotes(line, len, og_len);
	return (token);
}

char	*ft_special_join(char *arg, char *buff)
{
	char	*joined;

	joined = NULL;
	joined = ft_strjoin(arg, buff);
	free(arg);
	free(buff);
	return (joined);
}
