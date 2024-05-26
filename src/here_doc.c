/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 10:06:01 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/22 11:55:41 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expand_heredoc(t_redir **heredoc_redir)
{
    unlink(FNAME);
    ((*heredoc_redir)->here_doc_fd[W_HEREDOC]) = open(FNAME, (O_CREAT | O_WRONLY) | O_TRUNC, 0644);
    ((*heredoc_redir)->here_doc_fd[R_HEREDOC]) = open(FNAME, O_RDONLY | O_TRUNC);
    unlink(FNAME);
}

static t_bool is_delimiter(char *buffer, char *delimiter)
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

static t_bool ft_check_for_quotes(char *og_delimiter)
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

static char *get_value_in_between_quotes(char *line, char quote)
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

static char *expand_in_heredoc(t_exec *exec, char *line)
{
    int i;
    int len;
    char *buff;
    char *env_variable;
    char *expanded_line;

    if (!line)
        return NULL;
    i = 0;
    len = 0;
    expanded_line = NULL;
    env_variable = NULL;
    while(line[i])
    {
        len = 0;
        while (line[i + len] && line[i + len] != '$' && is_quote(line[i + len]) == FALSE)
            len++;
        if (is_quote(line[i + len]) == TRUE)
        {
            expanded_line = ft_strjoin(expanded_line, ft_substr(line, i, len + 1));
            i += len + 1;
            buff = get_value_in_between_quotes(line + i, line[i - 1]);
            if (buff)
                i += ft_strlen(buff);
            buff = expand_in_heredoc(exec, buff);
            expanded_line = ft_strjoin(expanded_line , buff);
        }
        else if (len)
        {
            expanded_line = ft_strjoin(expanded_line, ft_substr(line, i, len));
            i += len;
        }
        else if (line[i] == '$' && (line[i + 1]== '$'|| (ft_isalpha(line[i + 1]) == 0 && line[i + 1] != '_')))
        {
            if (line[i + 1]== '$')
            {
                expanded_line = ft_strjoin(expanded_line, ft_substr(line, i, 2));
                i += 2;
            }
            else 
            {
                expanded_line = ft_strjoin(expanded_line, ft_substr(line, i, 1));
                ++i;
            }
        }       
        else if (line[i] == '$')
        {
            i++;
            len = 0;
            while(line[i + len] && is_space(line[i + len]) == FALSE && is_quote(line[i + len]) == FALSE && line[i + len] != '$')
                ++len;
            if (len)
            {
                buff = ft_substr(line, i, len);
                env_variable = ft_get_value(exec, buff);
                // if (buff)
                //     printf("buff is %s and value \n", buff );
                expanded_line = ft_strjoin(expanded_line, env_variable);
                i += len;
                printf("line rest %s\n", &line[i]);
            }
        }
    }
    return expanded_line;
}

void here_doc_helper(t_exec *exec, int w_heredoc, char *og_delimiter, char *delimiter)
{
    char    *line;
    char    *buffer;
    t_bool  to_be_expanded;
    
    line = NULL;
    buffer = NULL;
    to_be_expanded = ft_check_for_quotes(og_delimiter);
    while(is_delimiter(line, delimiter) == FALSE)
    {
        if (buffer)
            free(buffer);
        line = readline(YELLOW"Heredoc>"RESET_COLORS);
        if (!line)
        {
            printf("\n");
            break;
        }
        if (to_be_expanded == TRUE && exec && is_delimiter(line, delimiter) == FALSE)
            buffer = expand_in_heredoc(exec, line);
        else if (is_delimiter(line, delimiter) == FALSE)
            buffer = ft_strdup(line);
        if (buffer)
            write(w_heredoc, buffer, ft_strlen(buffer));
    }
    close(w_heredoc);
}

void here_doc(t_exec *exec, t_redir *og_redir, t_redir *here_doc, t_bool error_flag)
{
    here_doc_helper(exec , here_doc->here_doc_fd[W_HEREDOC], og_redir->file_name, here_doc->file_name);
    if (error_flag == TRUE)
        close(here_doc->here_doc_fd[R_HEREDOC]);
}
