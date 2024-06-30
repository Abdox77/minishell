/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 05:27:22 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 05:27:26 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**initialize_args_if_null(char *cmd, char **args)
{
	static char	*default_args[2];

	if (args == NULL)
	{
		default_args[0] = cmd;
		default_args[1] = NULL;
		args = default_args;
	}
	return (args);
}

char	**expander(t_token *token, t_exec *exec, char *cmd)
{
	char	**args;
	char	**processed_args;
	char	**expanded_wildcards;

	args = initialize_args_if_null(cmd, token->cmd->args);
	processed_args = process_args(args, token, cmd, exec->env);
	expanded_wildcards = expand_wildcards(processed_args);
	free_strs(processed_args);
	// free_strs(args);
	return (expanded_wildcards);
}

void	cleanup_exec(t_exec *exec, char **args, int in, int out)
{
	free_strs(exec->envp);
	exec->envp = NULL;
	free_strs(args);
	reset_fd(in, out);
}
int	is_dir(char *path)
{
	struct stat buf;

	if (stat(path, &buf) == -1)
		return (0);
	else if (buf.st_mode & S_IFDIR)
		return (1);
	return (0);
}