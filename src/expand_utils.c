/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 04:54:08 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 04:55:37 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*find_env_value(const char *var_name, t_env *env_list)
{
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa_no_malloc(stat_handler(0, 0)));
	while (env_list != NULL)
	{
		if (strcmp(var_name, env_list->key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

char	**concat_args(char **args1, char **args2)
{
	int		len1;
	int		len2;
	char	**result;
	int		j;
	int		i;

	len1 = get_args_length(args1);
	len2 = get_args_length(args2);
	result = malloc_with_error((len1 + len2 + 1) * sizeof(char *));
	i = 0;
	while (i < len1)
	{
		result[i] = args1[i];
		i++;
	}
	j = 0;
	while (j < len2)
	{
		result[i + j] = args2[j];
		j++;
	}
	result[i + j] = NULL;
	return (result);
}

char	*get_var_value(int var_len, const char *var_start, t_env *env_list)
{
	char	*var_name;
	char	*var_value;

	var_name = ft_strndup(var_start, var_len);
	var_value = find_env_value(var_name, env_list);
	free(var_name);
	return (var_value);
}
