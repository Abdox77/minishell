# include "minishell.h"

char	**find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

void	free_2d(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	*get_cmd(char *cmd, char **envp)
{
	char	**tab;
	char	*pathcmd;
	int		j;

	j = 0;
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) != -1)
			return (ft_strdup(cmd));
		return (NULL);
	}
	tab = find_path(envp);
	if (!tab)
		return (printf("path not found\n"),
			exit(EXIT_FAILURE), NULL);
	while (tab[j] && tab)
	{
		pathcmd = ft_strjoin(tab[j], cmd);
		if (access(pathcmd, F_OK | X_OK) != -1)
			return (free_2d(tab), pathcmd);
		free(pathcmd);
		j++;
	}
	return (free_2d(tab), NULL);
}
