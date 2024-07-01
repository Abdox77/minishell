/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 05:22:45 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/01 23:13:06 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_left(t_token *node, int *fd, t_exec *exec)
{
	int	status;

	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	status = execute(node, exec);
	exit(status);
}

static void	execute_right(t_token *node, int *fd, t_exec *exec)
{
	int	status;

	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	status = execute(node, exec);
	exit(status);
}

void	close_fds(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

void	execute_pipe(t_token *node, t_exec *exec)
{
	int		status1;
	int		status2;
	pid_t	pid[2];
	int		fd[2];

	handle_signals_before(1);
	if (pipe(fd) == -1)
		return (perror("Pipe failed"), exit(EXIT_FAILURE),
			(void)0);
	pid[0] = fork();
	if (pid[0] == 0)
	{
		close(fd[0]);
		execute_left(node->l_token, fd, exec);
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		close(fd[1]);
		execute_right(node->r_token, fd, exec);
	}
	close_fds(fd);
	waitpid(pid[0], &status1, 0);
	waitpid(pid[1], &status2, 0);
	stat_handler(WEXITSTATUS(status2), 1);
}
