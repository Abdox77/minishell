/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 08:30:54 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 15:35:58 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_seperator(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	return (0);
}

static int	count(char *str, char *charset)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] != '\0' && check_seperator(str[i], charset))
			i++;
		if (str[i] != '\0')
			count++;
		while (str[i] != '\0' && !check_seperator(str[i], charset))
			i++;
	}
	return (count);
}

static int	segment_len(char *s, char *charset)
{
	int	len;

	len = 0;
	while (s[len] && !check_seperator(s[len], charset))
		len++;
	return (len);
}

static char	*ft_segdup(char *s, char *charset)
{
	int		i;
	int		len;
	char	*seg;

	i = 0;
	len = segment_len(s, charset);
	seg = (char *)malloc(sizeof(char) * (len + 1));
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

char	**split(char *str, char *charset)
{
	int		i;
	char	**splited;

	i = 0;
	if (!str)
		return (NULL);
	splited = (char **)malloc(sizeof(char *) * (count(str, charset) + 2));
	if (!splited)
		return (NULL);
	while (*str != '\0')
	{
		while (*str != '\0' && check_seperator(*str, charset))
			str++;
		if (*str != '\0')
		{
			splited[i] = ft_segdup(str, charset);
			i++;
		}
		while (*str && !check_seperator(*str, charset))
			str++;
	}
	splited[i] = NULL;
	return (splited);
}
