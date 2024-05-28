/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 15:22:09 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/04/14 15:27:17 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static void	set_empty_env(t_env **head)
{
    char *str;
	append_node(head, "SHLVL", "1");
	append_node(head, "PWD", getcwd(NULL, 0));
	str = ft_strjoin(getcwd(NULL, 0), "/minishell");
	append_node(head, "_", str);
	append_node(head, "PATH", "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
}
void	ft_write(char *str, int fd, int new_line)
{
	while (*str)
	{
		write (fd, str, 1);
		str++;
	}
	if (new_line == 1)
		write(fd, "\n", 1);
}

void    copy_key(char **env, t_var *var)
{
    var->tmp_key = ft_substr(env[var->i], 0, var->j);
    if (!var->tmp_key)
    {
        ft_write("Malloc error", 2, 1);
        exit(2);
    }
    var->rem = var->j + 1;
}

t_env    *parse_env(char **env)
{
    t_var    var;
    t_env    *head;

    head = NULL;
    var.i = 0;
    if (!*env)
		return (set_empty_env(&head), head);
    while (env[var.i])
    {
        var.j = 0;
        while (env[var.i][var.j] != '=')
            var.j++;
        if (env[var.i][var.j] == '=')
            copy_key(env, &var);
        var.j = 0;
        while (env[var.i][var.j])
            var.j++;
        var.tmp_val = ft_substr(env[var.i], var.rem, var.j - var.rem);
        if (!var.tmp_val)
        {
            ft_write("Malloc error", 2, 1);
            exit(2);
        }
        append_node(&head, var.tmp_key, var.tmp_val);
        free(var.tmp_val);
        free(var.tmp_key);
        var.i++;
    }
    // free(var.tmp_val);
    // free(var.tmp_key);
    return (head);
}

void print_env(t_env *head)
{
    while (head)
    {
        printf("%s=%s\n", head->key, head->value);
        head = head->next;
    }
}
