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

// char	*get_cmd(char *cmd, char **envp)
// {
// 	char	**tab;
// 	char	*pathcmd;
// 	int		j;

// 	j = 0;
// 	if (ft_strchr(cmd, '/'))
// 	{
// 		if (access(cmd, F_OK | X_OK) != -1)
// 			return (ft_strdup(cmd));
// 		return (NULL);
// 	}
// 	tab = find_path(envp);
// 	if (!tab)
// 		return (printf("path not found\n"),
// 			exit(EXIT_FAILURE), NULL);
// 	while (tab[j] && tab)
// 	{
// 		pathcmd = ft_strjoin("/", cmd);
// 		pathcmd = ft_strjoin(tab[j], pathcmd);
// 		if (access(pathcmd, F_OK | X_OK) != -1)
// 			return (free_strs(tab), pathcmd);
// 		free(pathcmd);
// 		j++;
// 	}
// 	return (free_strs(tab), NULL);
// }

char *get_cmd(char *cmd, char **envp)
{
    char **tab;
    char *pathcmd;
    char *tmp_pathcmd;
    int j;

    j = 0;
    if (ft_strchr(cmd, '/')) {
            return ft_strdup(cmd);
        return NULL;
    }
    tab = find_path(envp);
    if (!tab)
        return (printf("path not found\n"), exit(EXIT_FAILURE), NULL);
    while (tab[j] && tab) {
        tmp_pathcmd = ft_strjoin("/", cmd);
        pathcmd = ft_strjoin(tab[j], tmp_pathcmd);
        free(tmp_pathcmd);
        if (access(pathcmd, F_OK | X_OK) != -1)
            return (free_strs(tab), pathcmd);
        free(pathcmd);
        j++;
    }
    return (free_strs(tab), NULL);
}

static int	ft_nb_len(long n)
{
	int	len;

	len = 0;
	if (n <= 0)
	{
		len++;
		n = -n;
	}
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	ft_convert(char *s, int len, long n)
{
	int	sign;

	sign = 0;
	if (n < 0)
	{
		n = -n;
		sign = 1;
	}
	s[len--] = '\0';
	while (len >= 0)
	{
		s[len--] = (char)((n % 10) + 48);
		n /= 10;
	}
	if (sign)
		s[0] = '-';
}

char	*ft_itoa_no_malloc(int n)
{
	static char buffer[12]; // Enough to hold any 32-bit integer and the null terminator
	int	len;

	len = ft_nb_len((long)n);
	ft_convert(buffer, len, ((long)n));
	return buffer;
}
