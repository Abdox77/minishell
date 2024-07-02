/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_expand_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 05:11:59 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/02 23:24:56 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_expanded_length1(const char *str, t_env *env_list)
{
	size_t	length;

	length = 0;
	while (*str)
	{
		if (*str == '$')
			length += get_var_length(&str, env_list);
		else if (*str == '"')
			length += get_quoted_length(&str, env_list);
		else
		{
			length++;
			str++;
		}
	}
	return (length);
}

int	is_expandable_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '$');
}

int	is_quoted(const char *str)
{
	while (*str)
	{
		if (*str == '"' || *str == '\'')
			return (1);
		str++;
	}
	return (0);
}

char	*expand_string1(const char *str, t_env *env_list)
{
	size_t	result_size;
	char	*result;
	char	*result_ptr;

	result_size = get_expanded_length1(str, env_list) + 1;
	result = (char *)malloc(result_size);
	if (!result)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	result[0] = '\0';
	result_ptr = result;
	while (*str)
	{
		if (*str == '$')
			append_var_value(&result_ptr, &str, env_list);
		else if (*str == '"')
			append_quoted_value(&result_ptr, &str, env_list);
		else
			*result_ptr++ = *str++;
	}
	*result_ptr = '\0';
	return (result);
}
