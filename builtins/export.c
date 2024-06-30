/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:13:01 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 02:12:20 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

static char	*check_and_get_key(const char *p)
{
	int		i;
	char	*key;

	if (p == NULL)
		return (NULL);
	i = 0;
	if (!ft_isalpha((unsigned char)p[i]) && p[i] != '_')
		return (print_error((char *)p), NULL);
	while (p[i] && p[i] != '=' && !(p[i] == '+' && p[i + 1] == '='))
	{
		if (!ft_isalnum((unsigned char)p[i]) && p[i] != '_')
		{
			print_error((char *)p);
			return (NULL);
		}
		i++;
	}
	key = (char *)malloc((i + 1) * sizeof(char));
	if (!key)
		return (perror("malloc"), NULL);
	ft_strncpy(key, p, i);
	key[i] = '\0';
	return (key);
}

static t_env	*search_key(t_env *head, const char *key)
{
	t_env	*current;

	if (key == NULL)
		return (NULL);
	current = head;
	while (current != NULL)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static void	export_variable(t_env *env, char *arg)
{
	char	*key;
	t_env	*existing_node;

	key = check_and_get_key(arg);
	if (!key)
		return ;
	existing_node = search_key(env, key);
	if (!existing_node)
		handle_new_variable(&env, arg, key);
	else
	{
		handle_existing_variable(existing_node, arg);
		free(key);
	}
}

int	ft_export(t_env *env, char **args)
{
	int	i;

	if (!env || !args)
		return (-1);
	if (!args[1])
		return (print_export(env), 1);
	i = 1;
	while (args[i])
	{
		export_variable(env, args[i]);
		i++;
	}
	return (1);
}
