/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 04:44:14 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 04:59:38 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_variable_value(const char *var_value, char **result_ptr)
{
	if (var_value)
	{
		strcpy(*result_ptr, var_value);
		*result_ptr += strlen(var_value);
	}
}

int	is_inside_quotes(const char *str, const char *pos)
{
	int	in_single_quotes;
	int	in_double_quotes;

	in_single_quotes = 0;
	in_double_quotes = 0;
	while (str < pos)
	{
		if (*str == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (*str == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		str++;
	}
	return (in_double_quotes);
}

char	**single_arg(char *arg)
{
	char	**single_arg;

	single_arg = malloc_with_error(2 * sizeof(char *));
	single_arg[0] = ft_strdup(arg);
	single_arg[1] = NULL;
	return (single_arg);
}

char	**expand_arg_if_needed(char *arg, char *og_arg, t_env *env_list)
{
	char	*expanded;
	char	**split_expanded;

	if (arg)
	{
		if (ft_strchr(arg, '$'))
		{
			expanded = expand_string(og_arg, env_list);
			if (is_inside_quotes(og_arg, strchr(og_arg, '$')))
			{
				split_expanded = malloc_with_error(2 * sizeof(char *));
				split_expanded[0] = expanded;
				split_expanded[1] = NULL;
			}
			else
			{
				split_expanded = ft_split(expanded, ' ');
				free(expanded);
			}
			return (split_expanded);
		}
		return (single_arg(arg));
	}
	return (NULL);
}

int	get_args_length(char **args)
{
	int	length;

	length = 0;
	while (args[length] != NULL)
		length++;
	return (length);
}
