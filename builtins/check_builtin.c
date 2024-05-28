#include "../src/minishell.h"

// int	check_builtins(t_token *node, t_exec *exec, char **args)
int	check_builtins(char *cmd, t_exec *exec, char **args)
{
	if (!ft_strcmp(cmd, "echo"))
		return (ft_echo(args), 1);
	 if (!ft_strcmp(ft_tolowerr(cmd), "export"))
		return (ft_export(exec->env, args), 1);
	else if (!ft_strcmp(ft_tolowerr(cmd), "unset"))
		return (unset(&exec->env, args), 1);
	else if (!ft_strcmp(cmd, "pwd"))
		return (pwd(), 1);
	else if (!ft_strcmp(ft_tolowerr(cmd), "cd"))
		return (cd(args, exec), 1);
	else if (!ft_strcmp(ft_tolowerr(cmd), "env"))
		return (ft_env(exec->env), 1);
	else if (!ft_strcmp(ft_tolowerr(cmd), "exit"))
		return (ft_exit(args), 1);
	return (0);
}
