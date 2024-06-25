/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 08:11:48 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/25 01:32:20 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/minishell.h"

static int	check_key(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

// Helper function to check the key in the environment list and return its value
static char *get_value(t_env *env, const char *key)
{
    t_env *tmp = env;
    while (tmp)
	{
        if (ft_strcmp(tmp->key, key) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

// Helper function to update the value of a key in the environment list
static void set_value(t_env *env, const char *key, const char *value)
{
    t_env *tmp = env;
    while (tmp)
	{
        if (ft_strcmp(tmp->key, key) == 0)
		{
            // Free the old value and set the new value
            // free(tmp->value);
            tmp->value = ft_strdup(value);
            return;
        }
        tmp = tmp->next;
    }
	return;
    // If the key does not exist, create a new node
    // t_env *new_node = (t_env *)malloc(sizeof(t_env));
    // if (!new_node) {
    //     perror("malloc");
    //     return;
    // }
    // new_node->key = strdup(key);
    // new_node->value = strdup(value);
    // new_node->next = env->next;
    // env->next = new_node;
}

// Function to update PWD and OLDPWD after changing the directory
static void update_pwd_and_oldpwd(t_exec *exec)
{
    if (!exec || !exec->env)
        return;
    char *tmp_pwd = get_value(exec->env, "PWD");     // Find and save the current PWD value in tmp_pwd
    if (!tmp_pwd)
        return;         // if PWD does not exist in the list, return without doing anything
    char new_pwd[1024];
    if (getcwd(new_pwd, sizeof(new_pwd)) == NULL)
	{
        perror("getcwd");
        stat(1, 1);
        return;
    }
    set_value(exec->env, "PWD", new_pwd);
    set_value(exec->env, "OLDPWD", tmp_pwd);
}



int cd(char **arg, t_exec *exec)
{
	if (arg[0] == NULL)
	{
		if (!check_key(exec->env, "HOME"))
            return (stat(1,1), perror("cd: HOME not set"), 0);
		else
		{
			if (chdir(get_value(exec->env, "HOME")) == -1)
				return (stat(1,1), perror("cd"), 0);
			update_pwd_and_oldpwd(exec);
		}
	}
	else
	{
		if (chdir(arg[1]) == -1)
			return (stat(1,1), perror("cd"), 0);
		update_pwd_and_oldpwd(exec);
	}
    stat(0, 1);
	return (1);
}

// static char	*get_value(t_env *env, char *key)
// {
// 	t_env	*tmp;

// 	if (!env || !key)
// 		return (NULL);
// 	tmp = env;
// 	while (tmp)
// 	{
// 		if (!ft_strcmp(tmp->key, key))
// 			return (tmp->value);
// 		tmp = tmp->next;
// 	}
// 	return (NULL);
// }