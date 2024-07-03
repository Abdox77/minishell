/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:22:09 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/03 11:01:36 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	set_empty_env(t_env **head)
{
	char	*str;

	append_node(head, "SHLVL", "1");
	append_node(head, "PWD", getcwd(NULL, 0));
	str = ft_strjoin(getcwd(NULL, 0), "/minishell");
	append_node(head, "_", str);
}

static void	split_key_value(char **env, t_var *var)
{
	var->j = 0;
	while (env[var->i][var->j] != '=')
		var->j++;
	var->tmp_key = ft_substr(env[var->i], 0, var->j);
	var->rem = ++var->j;
	while (env[var->i][var->j])
		var->j++;
	var->tmp_val = ft_substr(env[var->i], var->rem, var->j - var->rem);
	if (!var->tmp_key || !var->tmp_val)
	{
		ft_write("Malloc error", 2, 1);
		exit(2);
	}
}

static void	add_env_node(t_env **head, t_var *var)
{
	append_node(head, var->tmp_key, var->tmp_val);
	free(var->tmp_val);
	free(var->tmp_key);
}

t_env	*parse_env(char **env)
{
	t_var	var;
	t_env	*head;

	head = NULL;
	var.i = 0;
	if (!*env)
		return (set_empty_env(&head), head);
	while (env[var.i])
	{
		split_key_value(env, &var);
		add_env_node(&head, &var);
		var.i++;
	}
	return (head);
}

void	print_env(t_env *head)
{
	while (head)
	{
		printf("%s=%s\n", head->key, head->value);
		head = head->next;
	}
}
