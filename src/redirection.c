/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 23:48:13 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/02 22:05:01 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_arg_if_needed1(char *arg, char *og_arg, t_env *env_list)
{
	char	*expanded;
	char	**exp;

	if (ft_strchr(arg, ' ') || ft_strchr(arg, '\t'))
		ambiguous();
	if (ft_strchr(arg, '*'))
		return (handle_wildcards(arg));
	if (ft_strchr(arg, '$'))
	{
		exp = expand_arg_if_needed(arg, og_arg, env_list);
		expanded = ft_strdup(exp[0]);
		if (exp && *exp && exp[1])
			ambiguous();
		free(exp);
		if ((!expanded || !*expanded) && arg && *arg)
			ambiguous();
		if (ft_strchr(expanded, ' ') || ft_strchr(expanded, '\t'))
			ambiguous();
		if (ft_strchr(expanded, '*'))
			return (handle_wildcards(expanded));
		return (expanded);
	}
	return (ft_strdup(arg));
}

void	handle_heredoc_expansion(t_redir *input, t_exec *exec)
{
	char	*line;
	char	*expanded_line;
	int		expanded_fd;

	unlink("FNAME");
	input->here_doc_fd[_HEREDOC_EXPAND_FD] = open("FNAME",
			O_CREAT | O_RDWR | O_TRUNC, 0644);
	unlink(FNAME);
	expanded_fd = input->here_doc_fd[_HEREDOC_EXPAND_FD];
	line = get_next_line(input->here_doc_fd[R_HEREDOC]);
	while (line)
	{
		expanded_line = expand_in_heredoc(exec, line);
		write(expanded_fd, expanded_line, ft_strlen(expanded_line));
		write(expanded_fd, "\n", 1);
		free(line);
		free(expanded_line);
		line = get_next_line(input->here_doc_fd[R_HEREDOC]);
	}
}

void	handle_output_redirections(t_redir *output, t_redir *og_output,
		t_env *env)
{
	char	*expanded_filename;

	while (output)
	{
		if (check_to_expand(output->file_name, env))
		{
			expanded_filename = expand_arg_if_needed1(output->file_name,
					og_output->file_name, env);
			open_output_file(output, expanded_filename);
			free(expanded_filename);
		}
		else
		{
			ft_write("ambiguous redirect", 2, 1);
			exit(1);
		}
		output = output->next;
		og_output = og_output->next;
	}
}

void	handle_redirections(t_redir *red, t_redir *og_red, t_env *env,
		t_exec *exec)
{
	t_redir	*redir;
	t_redir	*og_redir;
	char	*expanded_filename;

	redir = red;
	og_redir = og_red;
	while (redir)
	{
		if (redir->mode == INFILE)
			handle_infile(redir, og_redir, env);
		else if (redir->mode == HEREDOC)
			handle_heredoc(redir, og_redir, exec);
		else if (redir->mode == TRUNC || redir->mode == APPEND)
		{
			expanded_filename = expand_arg_if_needed1(redir->file_name,
					og_redir->file_name, env);
			open_output_file(redir, expanded_filename);
			free(expanded_filename);
		}
		redir = redir->next;
		og_redir = og_redir->next;
	}
}

void	handle_infile(t_redir *input, t_redir *og_input, t_env *env)
{
	int		fd;
	char	*expanded_filename;

	expanded_filename = expand_arg_if_needed1(input->file_name,
			og_input->file_name, env);
	fd = open(expanded_filename, O_RDONLY);
	if (fd < 0)
	{
		perror(expanded_filename);
		free(expanded_filename);
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 failed for input redirection");
		free(expanded_filename);
		exit(1);
	}
	close(fd);
	free(expanded_filename);
}
