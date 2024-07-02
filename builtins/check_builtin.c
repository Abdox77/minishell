/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 01:02:19 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/02 21:34:40 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	check_builtins(char *cmd, t_cmd *cc, t_exec *exc, char **args)
{
	if (is_builtin(cmd))
	{
		handle_redirections(cc->redir, cc->og_tokens->og_redir, exc->env, exc);
		if (!ft_strcmp(cmd, "echo"))
			return (ft_echo(args), 1);
		if (!ft_strcmp(ft_tolowerr(cmd), "export"))
			return (ft_export(exc->env, args), 1);
		else if (!ft_strcmp(ft_tolowerr(cmd), "unset"))
			return (unset(&exc->env, args), 1);
		else if (!ft_strcmp(cmd, "pwd"))
			return (pwd(), 1);
		else if (!ft_strcmp(ft_tolowerr(cmd), "cd"))
			return (cd(args, exc), 1);
		else if (!ft_strcmp(ft_tolowerr(cmd), "env"))
			return (ft_env(exc->env), 1);
		else if (!ft_strcmp(ft_tolowerr(cmd), "exit"))
			return (ft_exit(args), 1);
	}
	return (0);
}
