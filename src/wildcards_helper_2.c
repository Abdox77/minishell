/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_helper_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 22:15:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/07/02 22:23:22 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_bool	cmp_condition(char *dir_name, const char *pattern)
{
	if (is_match(dir_name, pattern)
		&& ft_strcmp(dir_name, ".") != 0
		&& ft_strcmp(dir_name, "..") != 0)
	{
		return (TRUE);
	}
	return (FALSE);
}
