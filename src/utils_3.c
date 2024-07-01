/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:22:04 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 13:22:33 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_dup_args(char **args)
{
	int		i;
	int		len;
	char	**new;

	if (!args)
		return (NULL);
	i = -1;
	len = strs_len(args);
	new = malloc(sizeof(char *) * (len + 1));
	if (!new)
		ft_error("malloc failed in dup args\n", EXIT_FAILURE);
	while (++i < len)
		new[i] = args[i];
	new[i] = NULL;
	return (new);
}

t_redir	*ft_dup_redir(t_redir *redir)
{
	t_redir	*tmp;
	t_redir	*new;

	if (!redir)
		return (NULL);
	new = malloc(sizeof(t_redir));
	if (!new)
		ft_error("malloc failed\n", EXIT_FAILURE);
	new = new_cmd_redir(redir->mode, redir->file_name);
	tmp = new;
	redir = redir->next;
	while (redir)
	{
		tmp->next = new_cmd_redir(redir->mode, redir->file_name);
		if (!tmp->next)
			ft_error("failed in dup redir\n", EXIT_FAILURE);
		redir = redir->next;
		tmp = tmp->next;
	}
	tmp->next = NULL;
	return (new);
}

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	i;
	char	*p;
	size_t	len;

	i = 0;
	len = 0;
	while (len < n && s1[len] != '\0')
		len++;
	p = (char *)malloc(sizeof(char) * (len + 1));
	if (!p)
		return (NULL);
	while (i < len)
	{
		p[i] = s1[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		++i;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
