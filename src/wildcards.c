/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 17:56:11 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 21:45:27 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_matches(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	size_t			count;

	dir = opendir(".");
	if (dir == NULL)
	{
		perror("opendir failed");
		return (0);
	}
	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (is_match(entry->d_name, pattern))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	if (count == 0)
		return (1);
	return (count);
}

static size_t	calculate_total_size(char **args)
{
	size_t	total_size;
	int		i;

	total_size = 0;
	i = 0;
	while (args[i] != NULL)
	{
		if ((ft_strchr(args[i], '*')) && !has_space_or_tab(args[i]))
			total_size += count_matches(args[i]);
		else
			total_size++;
		i++;
	}
	return (total_size);
}

static void	add_matches_to_expanded_args(char **expanded_args, char **matches,
		int *count, char *arg)
{
	int	j;

	j = 0;
	if (!matches || !matches[0])
		expanded_args[(*count)++] = ft_strdup(arg);
	else
	{	
		while (matches[j] != NULL)
		{
			expanded_args[(*count)++] = ft_strdup(matches[j]);
			free(matches[j]);
			j++;
		}
	}
	free(matches);
}

static char	**expand_args_with_matches(char **args, char **expanded_args)
{
	int		i;
	int		count;
	char	**matches;

	i = 0;
	count = 0;
	while (args[i] != NULL)
	{
		if ((strchr(args[i], '*')) && !has_space_or_tab(args[i]))
		{
			matches = list_matching_files(args[i]);
			// if (!matches)
			// {
			// 	matches = malloc(2 * sizeof(char *));
			// 	matches[0] = strdup(args[i]);
			// 	matches[1] = NULL;
				
			// }
			// // if (matches)
				add_matches_to_expanded_args(expanded_args, matches, &count, args[i]);
		}
		else
			expanded_args[count++] = strdup(args[i]);
		i++;
	}
	expanded_args[count] = NULL;
	return (expanded_args);
}

char	**expand_wildcards(char **args)
{
	size_t	total_size;
	char	**expanded_args;

	total_size = calculate_total_size(args);
	expanded_args = (char **)malloc((total_size + 1) * sizeof(char *));
	if (!expanded_args)
		return (perror("malloc failed"), NULL);
	return (expand_args_with_matches(args, expanded_args));
}
