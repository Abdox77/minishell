#include "minishell.h"

char **ft_dup_args(char **args)
{
	int i;
	int len;
	char **new;

	if (!args)
		return NULL;
	i = -1;
	len = strs_len(args);
	if (len == 0)
		printf("len is 0 in dup_args\n");
	new = malloc (sizeof(char *) * (len + 1));
	if (!new)
		ft_error("malloc failed in dup args\n", EXIT_FAILURE);
	while(++i < len)
		new[i] = args[i];
	new[i] = NULL;
	return (new);
}

t_redir *ft_dup_redir(t_redir *redir)
{
	t_redir *tmp;
	t_redir *new;
	
	if (!redir)
		return NULL;
	new = malloc(sizeof(t_redir));
	if (!new)
		ft_error("malloc failed\n", EXIT_FAILURE);
	new = new_cmd_redir(redir->mode, redir->file_name);
	tmp = new;
	redir = redir->next;
	while(redir)
	{
		tmp->next = new_cmd_redir(redir->mode, redir->file_name);
		if (!tmp->next)
			ft_error("failed in dup redir\n", EXIT_FAILURE);
		redir = redir->next;
		tmp = tmp->next;
	}
	tmp->next = NULL; // just in case 
	return (new);
}

