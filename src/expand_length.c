/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_length.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 04:47:08 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/02 23:25:03 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	handle_quotes_and_length(const char **str, int *in_single_quotes,
		int *in_double_quotes)
{
	if (**str == '\'' && !*in_double_quotes)
	{
		*in_single_quotes = !*in_single_quotes;
		(*str)++;
		return (0);
	}
	else if (**str == '"' && !*in_single_quotes)
	{
		*in_double_quotes = !*in_double_quotes;
		(*str)++;
		return (0);
	}
	return (1);
}

size_t	handle_dollar_sign(const char **str, t_env *env_list, size_t length)
{
	const char	*var_start;
	char		*var_value;
	size_t		var_len;

	(*str)++;
	var_start = *str;
	if (**str == '?')
		handle_special_variable(str, env_list, &length);
	else
	{
		while (**str && (ft_isalnum((unsigned char)**str) || **str == '_'))
			(*str)++;
		if (*str == var_start)
			length++;
		else
		{
			var_len = *str - var_start;
			var_value = get_var_value(var_len, var_start, env_list);
			if (var_value)
				length += ft_strlen(var_value);
		}
	}
	return (length);
}

static char	*find_env_value(const char *var_name, t_env *env_list)
{
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa_no_malloc(stat_handler(0, 0)));
	while (env_list != NULL)
	{
		if (ft_strcmp(var_name, env_list->key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

size_t	get_expanded_length(const char *str, t_env *env_list)
{
	size_t	length;
	int		in_single_quotes;
	int		in_double_quotes;

	length = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (*str)
	{
		if (!handle_quotes_and_length(&str, &in_single_quotes,
				&in_double_quotes))
			continue ;
		else if (*str == '$' && !in_single_quotes)
			length = handle_dollar_sign(&str, env_list, length);
		else
		{
			length++;
			str++;
		}
	}
	return (length);
}

void	handle_special_variable(const char **str, t_env *env_list,
		size_t *length)
{
	const char	*var_value = find_env_value("?", env_list);

	*length += ft_strlen(var_value);
	(*str)++;
}
