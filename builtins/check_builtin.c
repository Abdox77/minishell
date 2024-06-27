#include "../src/minishell.h"


int is_builtin(char *cmd)
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

int	check_builtins(char *cmd, t_cmd *cc, t_exec *exec, char **args)
{
	if(is_builtin(cmd))
	{
		handle_redirections(cc, exec->env, exec);
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
	}
	return (0);
}
