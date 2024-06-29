/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:51:25 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 18:23:47 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_lvars(t_lvars *vars)
{
	vars->len = 0;
	vars->og_len = 0;
}

t_bool	is_parenthesis(char c)
{
	if (c == '(' || c == ')')
		return (TRUE);
	return (FALSE);
}

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	memset(cmd, 0, sizeof(t_cmd));
	return (cmd);
}

void	skip_spaces(char **line)
{
	if (!*line || !**line)
		return ;
	while (**line && is_space(**line) == TRUE)
		++(*line);
}

t_bool	check_char_validation_for_cmd(char **line, char *tmp, int len)
{
	if (tmp[len] && is_special_char(tmp[len]) == FALSE && is_an_operator(line,
			len) == FALSE && is_parenthesis(tmp[len]) == FALSE)
	{
		return (TRUE);
	}
	return (FALSE);
}
