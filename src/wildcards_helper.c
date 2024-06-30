/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 17:58:31 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 18:21:30 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_match_util(const char *p)
{
	while (*p)
	{
		if (*p != '*')
			return (false);
		p++;
	}
	return (true);
}

bool	is_match(const char *s, const char *p)
{
	const char	*ws;
	const char	*wp;

	ws = 0;
	wp = 0;
	while (*s)
	{
		if (*p == *s && (s++))
			p++;
		else if (*p == '*')
		{
			wp = ++p;
			ws = s;
		}
		else if (wp)
		{
			p = wp;
			s = ++ws;
		}
		else
			return (false);
	}
	return (is_match_util(p));
}

int	has_space_or_tab(const char *str)
{
	while (*str)
	{
		if (*str == ' ' || *str == '\t')
			return (1);
		str++;
	}
	return (0);
}

char	**allocate_matches(size_t count)
{
	char	**matches;

	matches = (char **)malloc((count + 1) * sizeof(char *));
	if (!matches)
	{
		perror("malloc failed");
		return (NULL);
	}
	return (matches);
}

char	**list_matching_files(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	size_t			count;
	size_t			idx;

	count = count_matches(pattern);
	matches = allocate_matches(count);
	if (!matches)
		return (NULL);
	dir = opendir(".");
	if (dir == NULL)
		return (perror("opendir failed"), free(matches), NULL);
	idx = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (is_match(entry->d_name, pattern))
			matches[idx++] = strdup(entry->d_name);
		entry = readdir(dir);
	}
	matches[idx] = NULL;
	closedir(dir);
	return (matches);
}
