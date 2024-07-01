/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:00:53 by amohdi            #+#    #+#             */
/*   Updated: 2024/07/01 22:34:57 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_iter_vars(t_iter_vars *vars, char **expanded_line)
{
	*expanded_line = NULL;
	vars->i = 0;
	vars->len = 0;
}

static void	heredoc_to_be_continued(t_token **root, enum e_bool error_flag,
		int pid)
{
	int	status;

	waitpid(pid, &status, WUNTRACED);
	sig_state(SET_SIGS, _SAVE);
	if (WEXITSTATUS(status) == 120)
		ft_print_error(NULL, NULL, RESET_HEREDOC);
	heredoc_to_fd_to_infiles(&(*root)->l_token, error_flag);
	heredoc_to_fd_to_infiles(&(*root)->r_token, error_flag);
}

void	heredoc_to_fd_to_infiles(t_token **root, enum e_bool error_flag)
{
	t_redir	*tmp;
	pid_t	pid;

	if (!*root)
		return ;
	sig_state(UNSET_SIGS, _SAVE);
	pid = fork();
	if (pid == 0)
	{
		child_singal_handler();
		if ((*root)->type == CMD && (*root)->cmd->redir)
		{
			tmp = (*root)->cmd->redir;
			while (tmp)
			{
				if (tmp->mode == HEREDOC && tmp->file_name)
					here_doc(tmp, error_flag);
				tmp = tmp->next;
			}
		}
		exit(0);
	}
	heredoc_to_be_continued(root, error_flag, pid);
}

void	heredoc_to_fds(t_token **root)
{
	t_redir	*tmp;

	if (!*root)
		return ;
	if ((*root)->type == CMD && (*root)->cmd->redir)
	{
		tmp = (*root)->cmd->redir;
		while (tmp)
		{
			if (tmp->mode == HEREDOC)
				heredoc_to_fd(&tmp);
			tmp = tmp->next;
		}
	}
	heredoc_to_fds(&((*root)->l_token));
	heredoc_to_fds(&((*root)->r_token));
}
