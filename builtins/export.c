/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:13:01 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/27 18:51:39 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

void print_error(char *str)
{
	stat_handler(1, 1);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
}
static void	ft_lstadd_back_env(t_env **head, t_env *new_node)
{
	t_env *current;

	if (!new_node)
		return;
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

static char	*ft_strncpy(char *dest, const char *src, unsigned int n)
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

static char	*search_for_value(const char *p)
{
	char	*equal_sign;
	char	*value_start;
	char		*value;

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

static void	swap(t_env *tmp)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = tmp->key;
	tmp->key = tmp->next->key;
	tmp->next->key = temp_key;
	temp_value = tmp->value;
	tmp->value = tmp->next->value;
	tmp->next->value = temp_value;
}

static t_env	*sort_export(t_env *env)
{
	t_env	*tmp;
	int		swapped;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		tmp = env;
		while (tmp->next)
		{
			if (ft_strcmp(tmp->key, tmp->next->key) > 0)
			{
				swap(tmp);
				swapped = 1;
			}
			tmp = tmp->next;
		}
	}
	return (env);
}

static void	print_export(t_env *env)
{
	t_env	*tmp;

	tmp = sort_export(env);
	while (tmp)
	{
		if (!tmp->value)
			printf("declare -x %s\n", tmp->key);
		else
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

static char	*ft_j_strcat(char *s1, const char *s2, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*p;
	size_t	i;
	size_t	l1;
	size_t	l2;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		return (ft_strdup(s2));
	else if (!s2 && s1)
		return (ft_strdup(s1));
	i = 0;
	l1 = ft_strlen((char *)s1);
	l2 = ft_strlen((char *)s2);
	p = (char *)malloc(sizeof(char) * (l1 + l2 + 1));
	if (!p)
		return (NULL);
	while (i < l1)
	{
		p[i] = ((char *)s1)[i];
		i++;
	}
	ft_j_strcat((p + i), s2, l2);
	return (p);
}

static int	handle_new_variable(t_env **env, const char *arg, char *key)
{
	t_env *new_node;

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

static void	handle_existing_variable(t_env *existing_node, const char *arg)
{
	char	*new_value;
	char	*concatenated_value;

	if (strstr(arg, "+="))
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

// int	ft_export(t_env *env, char **args)
// {
// 	int		i;
// 	char	*key;
// 	t_env	*existing_node;

// 	if (env == NULL || args == NULL)
// 		return (-1);
// 	else if (args[1] == NULL)
// 		return (print_export(env), 1);
// 	i = 1;
// 	while (args[i])
// 	{
// 		key = check_and_get_key(args[i]);
// 		if (key == NULL)
// 		{
// 			i++;
// 			continue;
// 		}
// 		existing_node = search_key(env, key);
// 		if (existing_node == NULL)
// 			handle_new_variable(&env, args[i], key);
// 		else
// 		{
// 			handle_existing_variable(existing_node, args[i]);
// 			free(key);
// 		}
// 		i++;
// 	}
// 	return (1);
// }
// int	ft_export(t_env *env, char **args)
// {
// 	int		i;
// 	char	*key;
// 	t_env	*existing_node;

// 	if (!env || !args)
// 		return (-1);
// 	if (!args[1])
// 		return (print_export(env), 1);
// 	i = 1;
// 	while (args[i])
// 	{
// 		key = check_and_get_key(args[i]);
// 		if (!key)
// 		{
// 			i++;
// 			continue;
// 		}
// 		existing_node = search_key(env, key);
// 		if (!existing_node)
// 			handle_new_variable(&env, args[i], key);
// 		else
// 		{
// 			handle_existing_variable(existing_node, args[i]);
// 			free(key);
// 		}
// 		i++;
// 	}
// 	return (1);
// }

static void	export_variable(t_env *env, char *arg)
{
	char	*key;
	t_env	*existing_node;

	key = check_and_get_key(arg);
	if (!key)
		return;
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
	int		i;

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
