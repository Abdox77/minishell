/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 04:36:13 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/02 22:03:57 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_special_variable2(const char **str, char **result_ptr,
		t_env *env_list)
{
	const char	*var_value = find_env_value("?", env_list);

	copy_variable_value(var_value, result_ptr);
	(*str)++;
}

void	extract_variable_name(const char **str, char **var_name_ptr)
{
	const char	*var_start = *str;
	size_t		var_len;

	while (**str && (isalnum((unsigned char)**str) || **str == '_'))
		(*str)++;
	var_len = *str - var_start;
	if (var_len > 0)
		*var_name_ptr = ft_strndup(var_start, var_len);
}

void	handle_variable_expansion(const char **str, char **result_ptr,
		t_env *env_list)
{
	char		*var_name;
	const char	*var_value;

	var_name = NULL;
	extract_variable_name(str, &var_name);
	if (var_name)
	{
		var_value = find_env_value(var_name, env_list);
		copy_variable_value(var_value, result_ptr);
		free(var_name);
	}
	else
		*(*result_ptr)++ = '$';
}

void	expand_variable(const char **str, char **result_ptr, t_env *env_list,
		int *in_single_quotes)
{
	if (**str == '?')
		handle_special_variable2(str, result_ptr, env_list);
	else if (!*in_single_quotes)
		handle_variable_expansion(str, result_ptr, env_list);
	else
		*(*result_ptr)++ = '$';
}
