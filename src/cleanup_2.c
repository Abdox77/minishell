/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 06:13:40 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 06:14:23 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	if (strs)
	{
		while (strs[i])
		{
			safe_free(strs[i]);
			strs[i] = NULL;
			i++;
		}
		free(strs);
		strs = NULL;
	}
}

void	safe_free(void *ptr)
{
	if (!ptr)
		return ;
	else
	{
		free(ptr);
		ptr = NULL;
	}
}

void	free_redirections(t_redir *redir)
{
	t_redir	*tmp;

	if (!redir)
		return ;
	tmp = redir;
	while (tmp)
	{
		tmp = redir->next;
		safe_free(redir->file_name);
		safe_free(redir);
		redir = tmp;
	}
}

void	free_expands(t_expands *expands)
{
	if (!expands)
		return ;
	safe_free(expands->og_cmd);
	free_strs(expands->og_args);
	expands->og_args = NULL;
	free_redirections(expands->og_redir);
	safe_free(expands);
}
