/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 01:19:41 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/07/02 22:20:21 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

void	print_error(char *str)
{
	stat_handler(1, 1);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}

static void	ft_lstadd_back_env(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!new_node)
		return ;
	if (*head == NULL)
		*head = new_node;
	else
	{
		current = *head;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}

int	handle_new_variable(t_env **env, const char *arg, char *key)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (new_node == NULL)
	{
		perror("malloc");
		free(key);
		return (-1);
	}
	new_node->key = key;
	new_node->value = search_for_value(arg);
	new_node->next = NULL;
	ft_lstadd_back_env(env, new_node);
	return (1);
}

char	*search_for_value(const char *p)
{
	char	*equal_sign;
	char	*value_start;
	char	*value;

	if (p == NULL)
		return (NULL);
	equal_sign = ft_strchr(p, '=');
	if (!equal_sign)
		return (NULL);
	value_start = equal_sign + 1;
	value = (char *)malloc((ft_strlen(value_start) + 1) * sizeof(char));
	if (!value)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strcpy(value, value_start);
	return (value);
}

void	handle_existing_variable(t_env *existing_node, const char *arg)
{
	char	*new_value;
	char	*concatenated_value;

	if (arg && ft_strnstr(arg, "+=", ft_strlen(arg)))
	{
		new_value = search_for_value(arg);
		if (new_value)
		{
			concatenated_value = ft_strjoin(existing_node->value, new_value);
			free(existing_node->value);
			free(new_value);
			existing_node->value = concatenated_value;
		}
	}
	else
	{
		new_value = search_for_value(arg);
		if (new_value)
		{
			free(existing_node->value);
			existing_node->value = new_value;
		}
	}
}
