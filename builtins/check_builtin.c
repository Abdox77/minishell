#include "../src/minishell.h"

int	check_builtins(t_token *node, t_exec *exec)
{
	if (!ft_strcmp(node->cmd->cmd, "echo"))
		return (ft_echo(node->cmd->args), 1);
	// else if (!ft_strcmp(ft_tolowerr(node->cmd->cmd), "export"))
	// 	return (ft_export(exec->env, node->cmd->args), 1);
	else if (!ft_strcmp(ft_tolowerr(node->cmd->cmd), "unset"))
		return (unset(&exec->env, node->cmd->args), 1);
	else if (!ft_strcmp(node->cmd->cmd, "pwd"))
		return (pwd(), 1);
	else if (!ft_strcmp(ft_tolowerr(node->cmd->cmd), "cd"))
		return (cd(node->cmd->args, exec), 1);
	else if (!ft_strcmp(ft_tolowerr(node->cmd->cmd), "env"))
		return (ft_env(exec->env), 1);
	// else if (!ft_strcmp(ft_tolower(node->cmd->cmd), "exit"))
	// 	return (ft_exit());
	return (0);
}