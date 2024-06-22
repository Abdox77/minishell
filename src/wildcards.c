#include "minishell.h"
#include <dirent.h>
#include <stdbool.h>

bool	is_match(const char *s, const char *p)
{
	const char	*ws = 0;
	const char	*wp = 0;

	while (*s)
	{
		if (*p == *s || *p == '?')
			s++, p++;
		else if (*p == '*')
			wp = ++p, ws = s;
		else if (wp)
			p = wp, s = ++ws;
		else
			return (false);
	}
	while (*p)
	{
		if (*p != '*')
			return (false);
		p++;
	}
	return (true);
}

static size_t	count_matches(const char *pattern)
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
	while ((entry = readdir(dir)) != NULL)
	{
		if (is_match(entry->d_name, pattern))
			count++;
	}
	closedir(dir);
	return (count);
}

static char	**allocate_matches(size_t count)
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

static char	**list_matching_files(const char *pattern)
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
	{
		perror("opendir failed");
		return (free(matches), NULL);
	}
	idx = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (is_match(entry->d_name, pattern))
			matches[idx++] = strdup(entry->d_name);
	}
	matches[idx] = NULL;
	closedir(dir);
	return (matches);
}

static int	has_space_or_tab(const char *str)
{
	while (*str)
	{
		if (*str == ' ' || *str == '\t')
			return (1);
		str++;
	}
	return (0);
}

static size_t	calculate_total_size(char **args)
{
	size_t	total_size;
	int		i;

	total_size = 0;
	i = 0;
	while (args[i] != NULL)
	{
		if ((strchr(args[i], '*') || strchr(args[i], '?')) &&
			!has_space_or_tab(args[i]))
			total_size += count_matches(args[i]);
		else
			total_size++;
		i++;
	}
	return (total_size);
}

static void	add_matches_to_expanded_args(char **expanded_args, char **matches, int *count)
{
	int	j;

	j = 0;
	while (matches[j] != NULL)
	{
		expanded_args[(*count)++] = strdup(matches[j]);
		free(matches[j]);
		j++;
	}
	free(matches);
}

static char	**expand_args_with_matches(char **args, char **expanded_args)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (args[i] != NULL)
	{
		if ((strchr(args[i], '*') || strchr(args[i], '?')) &&
			!has_space_or_tab(args[i]))
		{
			char	**matches = list_matching_files(args[i]);
			if (matches)
				add_matches_to_expanded_args(expanded_args, matches, &count);
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

void	free_expanded_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i] != NULL)
	{
		free(args[i]);
		i++;
	}
	free(args);
}
