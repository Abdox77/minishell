/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 20:56:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/28 21:08:21 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void heredoc_to_fd(t_redir **heredoc_redir)
{
    unlink(FNAME);
    ((*heredoc_redir)->here_doc_fd[W_HEREDOC]) = open(FNAME, (O_CREAT | O_WRONLY) | O_TRUNC, 0644);
    ((*heredoc_redir)->here_doc_fd[R_HEREDOC]) = open(FNAME, O_RDONLY | O_TRUNC);
    unlink(FNAME);
}

t_bool is_delimiter(char *buffer, char *delimiter)
{
    int i;

    if (!buffer || !delimiter)
        return FALSE;
    i = 0;
    while(buffer[i] && delimiter[i] && buffer[i] == delimiter[i])
        ++i;
    if (delimiter[i] || buffer[i])
        return (FALSE);
    return (TRUE);
}

t_bool ft_check_for_quotes(char *og_delimiter)
{
    int     i;

    if (!og_delimiter)
        return FALSE;
    i = -1;
    while(og_delimiter[++i])
        if (is_quote(og_delimiter[i]) == TRUE)
            return FALSE;
    return TRUE;
}

char *ft_get_value(t_exec *exec, char *key)
{
    t_env *env;
    if (!exec || (exec && !exec->env))
        return (free(key), NULL);
    env = exec->env;
    while(env)
    {
        if (env->key && ft_strcmp(env->key, key) == 0)
            return (free(key), ft_strdup(env->value));
        env = env->next;
    }
    return (NULL);
}

char *get_value_in_between_quotes(char *line, char quote)
{
    int i;
    int len;
    char *token;

    i = -1;
    len = 0;
    if (!line || !*line)
        return NULL;
    while (line[len] && line[len] != quote)
        len++;
    if (!line[len])
    {
        if (0 == len && line[len] == quote)
            ++line;
        return NULL;
    }
    token = malloc(sizeof(char) * len + 1);
    if(!token)
        return (NULL);
    while(++i < len)
        token[i] = line[i];
    token[i] = '\0';
    return (token);
}
