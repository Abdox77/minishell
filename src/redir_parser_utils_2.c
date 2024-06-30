/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser_utils_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 20:46:17 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 13:04:57 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_redir_mode	get_redir_mode(char **line)
{
	enum e_redir_mode	mode;

	if (**line == '<')
	{
		mode = INFILE;
		++(*line);
		if (**line == '<')
		{
			++(*line);
			mode = HEREDOC;
		}
	}
	else
	{
		mode = TRUNC;
		++(*line);
		if (**line == '>')
		{
			++(*line);
			mode = APPEND;
		}
	}
	special_trim(line);
	return (mode);
}
