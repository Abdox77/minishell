/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 10:06:01 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/28 21:03:29 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_valid_char_for_expansion(char c)
{
	if (c != '\0' && c != '$' && is_quote(c) == FALSE)
		return (TRUE);
	return (FALSE);
}

void	here_doc_helper(int w_heredoc, char *delimiter)
{
	char	*line;

	line = NULL;
	while (1337)
	{
		free(line);
		line = readline(YELLOW "Heredoc>" RESET_COLORS);
		if (!line)
		{
			printf("\n");
			break ;
		}
		if (is_delimiter(line, delimiter) == FALSE)
		{
			write(w_heredoc, line, ft_strlen(line));
			write(w_heredoc, "\n", 1);
		}
		else
		{
			free(line);
			break ;
		}
	}
	close(w_heredoc);
}

void	here_doc(t_redir *here_doc, t_bool error_flag)
{
	here_doc_helper(here_doc->here_doc_fd[W_HEREDOC], here_doc->file_name);
	if (error_flag == TRUE)
	{
		close(here_doc->here_doc_fd[R_HEREDOC]);
		if (here_doc->to_be_expanded == TRUE)
			close(here_doc->here_doc_fd[_HEREDOC_EXPAND_FD]);
	}
}
