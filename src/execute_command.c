/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 05:25:05 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/01 14:13:17 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_2_strs(char **str1, char **str2)
{
	free_strs(str1);
	free_strs(str2);
}

int	handle_fork_execution(t_token *token, t_exec *exec, t_cmd_args *st)
{
	char	*cmd_path;
	int		ex;

	handle_signals();
	handle_redirections(token->cmd->redir, token->cmd->og_tokens->og_redir,
		exec->env, exec);
	if (!st->flag || !token->cmd->og_tokens->og_cmd)
	{
		stat_handler(0, 1);
		exit(0);
	}
	cmd_path = get_cmd(st->cmd, exec->envp);
	execve(cmd_path, st->args, exec->envp);
	ex = 127;
	if (access(cmd_path, F_OK) == -1)
		ex = 127;
	else if (access(cmd_path, X_OK) == -1)
		ex = 126;
	if (is_dir(cmd_path))
		ex = 126;
	exec_error(st->cmd, cmd_path);
	free(cmd_path);
	free_2_strs(st->args, exec->envp);
	exit(ex);
}

void	exec_error(char *cmd, char *cmd_path)
{
	ft_write("minishell: ", 2, 0);
	ft_write(cmd, 2, 0);
	ft_write(": ", 2, 0);
	if (!*cmd)
		ft_write("command not found\n", 2, 0);
	else if (is_dir(cmd_path))
	{
		ft_write("Is a directory\n", 2, 0);
	}
	else
	{
		if (access(cmd_path, F_OK) == -1)
		{
			if (!ft_strchr(cmd, '/'))
				ft_write("command not found\n", 2, 0);
			else
				ft_write("No such file or directory\n", 2, 0);
		}
		else if (access(cmd_path, X_OK) == -1)
			ft_write("Permission denied\n", 2, 0);
	}
}

// static int	handle_builtin_or_fork(t_token *token, t_exec *exec, char *cmd,
// 		char **args, int flag)
static int	handle_builtin_or_fork(t_token *token, t_exec *exec, t_cmd_args *st)
{
	int	status;

	if (check_builtins(st->cmd, token->cmd, exec, st->args))
		return (stat_handler(0, 0));
	else
	{
		if (fork() == 0)
			handle_fork_execution(token, exec, st);
		wait(&status);
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				write(1, "Quit (core dumped)\n", 20);
			return (128 + WTERMSIG(status));
		}
		return (WEXITSTATUS(status));
	}
}

int	execute_command(t_token *token, t_exec *exec)
{
	int			in;
	int			out;
	int			status;
	t_cmd_args	cmd_args;

	exec->envp = env_to_envp(exec);
	handle_signals_before(0);
	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);
	cmd_args.cmd = token->cmd->cmd;
	cmd_args.args = expander(token, exec, cmd_args.cmd);
	cmd_args.flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);
	if (cmd_args.args[0])
		cmd_args.cmd = cmd_args.args[0];
	else
		cmd_args.cmd = "\0";
	if (!*cmd_args.args && token->cmd->cmd)
		status = 0;
	else
		status = handle_builtin_or_fork(token, exec, &cmd_args);
	cleanup_exec(exec, cmd_args.args, in, out);
	return (stat_handler(status, 1), status);
}
