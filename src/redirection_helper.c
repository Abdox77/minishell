/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 23:51:02 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 23:58:19 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ambiguous(void)
{
	ft_write("ambiguous redirect", 2, 1);
	exit(1);
}

char	*handle_wildcards(char *arg)
{
	char	**tmp;
	char	**tmp2;
	char	*expanded;

	tmp = initialize_args_if_null(arg, NULL);
	tmp2 = expand_wildcards(tmp);
	if (!tmp2 || !*tmp2)
		return (ft_strdup(tmp[0]));
	if (tmp2[1])
		ambiguous();
	expanded = ft_strdup(tmp2[0]);
	free(tmp2);
	return (expanded);
}

void	handle_input_redirections(t_redir *input, t_redir *og_input, t_env *env,
		t_exec *exec)
{
	while (input)
	{
		if (input->mode == INFILE)
			handle_infile(input, og_input, env);
		else if (input->mode == HEREDOC)
			handle_heredoc(input, og_input, exec);
		input = input->next;
		og_input = og_input->next;
	}
}

void	handle_heredoc(t_redir *input, t_redir *og_input, t_exec *exec)
{
	int	duppy;

	if (og_input->to_be_expanded == TRUE)
	{
		handle_heredoc_expansion(input, exec);
		duppy = open("FNAME", O_RDWR, 0644);
		unlink("FNAME");
		if (dup2(duppy, STDIN_FILENO) < 0)
		{
			perror("dup2 failed for input redirection");
			exit(1);
		}
		close(input->here_doc_fd[_HEREDOC_EXPAND_FD]);
		close(input->here_doc_fd[R_HEREDOC]);
	}
	else
	{
		if (dup2(input->here_doc_fd[0], STDIN_FILENO) < 0)
		{
			perror("dup2 failed for input redirection");
			exit(1);
		}
		close(input->here_doc_fd[0]);
	}
}

void	open_output_file(t_redir *output, char *expanded_filename)
{
	int	fd;

	fd = -1;
	if (output->mode == TRUNC)
		fd = open(expanded_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (output->mode == APPEND)
		fd = open(expanded_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(expanded_filename);
		free(expanded_filename);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2 failed for output redirection");
		free(expanded_filename);
		exit(EXIT_FAILURE);
	}
	close(fd);
}
