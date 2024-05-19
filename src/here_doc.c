/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 10:06:01 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/19 13:13:57 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *get_rand_file_name_2(void)
{
    char        *buff;
    char        *fname;
    static int  i;
    
    buff = ft_itoa(++i);
    if (buff == NULL)
        return (NULL);
    fname = ft_strjoin("HERE_DOC_", buff);
    return fname;
}

static char *get_rand_file_name(void)
{
    int fd;
    int bytes;
    char *buff;
    
    fd = open("/dev/random", O_RDWR);
    if (fd < 0)
        return (NULL);
    buff = malloc(sizeof(char) * 11);
    if (NULL == buff)
        return (close(fd), NULL);
    bytes = read(fd, buff, 10);
    if (bytes <= 0)
        return (close(fd), free(buff), NULL);
    buff[bytes] = '\0';
    return (close(fd), buff);
}

char *generate_file_name(void)
{
    char    *fname;

    fname = get_rand_file_name();
    if (fname == NULL)
        fname = get_rand_file_name_2();
    if (fname == NULL)
        ft_print_error("May god be with you\n", NULL, SAVE);
    return fname;
}

void expand_heredoc(t_redir **heredoc_redir)
{
    char *fname;

    fname = generate_file_name();
    if (NULL == fname) 
        return;
    
    printf("fname is %s\n", fname);
    ((*heredoc_redir)->here_doc_fd[W_HEREDOC]) = open(fname, O_CREAT | O_RDWR | O_TRUNC, 0644);
    printf("fd_w %d\n",  ((*heredoc_redir)->here_doc_fd[W_HEREDOC]));
    ((*heredoc_redir)->here_doc_fd[R_HEREDOC]) = open(fname, O_RDWR);
    printf("fd_r %d\n",  ((*heredoc_redir)->here_doc_fd[R_HEREDOC]));
    unlink(fname);
    free(fname);
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
        if (is_quote(og_delimiter[++i]) == TRUE)
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

static char *expand_in_heredoc(t_exec *exec, char *line)
{
    int i;
    int len;
    char *env_variable;
    char *expanded_line;

    if (!line)
        return NULL;
    i = 0;
    len = 0;
    expanded_line = NULL;
    env_variable = NULL;
    printf("line is before SEGV %s\n", line);
    if (!line)
        printf("oooops\n");
    while(line[i])
    {
        len = 0;
        while (line[i + len] && line[i + len] != '$')
            len++;
        if (len && line[i + len] && line[i + len + 1])
            expanded_line = ft_strjoin(expanded_line, ft_substr(line, i, len));
        else if (line[i + len] && !line[i + len + 1])
            expanded_line = ft_strjoin(expanded_line, ft_substr(line, i, len + 1));
        i += len;
        if (line[i] == '$')
        {
            ++i;
            len = 0;
            while(line[i + len] && is_space(line[i + len]) == FALSE && line[i + len] != '$')
                ++len;
            if (line[i + len] != '$')
            {
                char *buff = ft_substr(line, i, len);
                printf("buff is %s\n", buff);
                env_variable = ft_get_value(exec, buff);
                expanded_line = ft_strjoin(expanded_line, env_variable);
            }
            else if (!len)

        }
        // if (line[i])
            // ++i;
    }
    printf("line is after got expanded : env_var %s || %s\n", env_variable, expanded_line);
    return expanded_line;
}

static void here_doc_helper(t_exec *exec, int w_heredoc, char *og_delimiter, char *delimiter)
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
        printf("line is |%s|     |%s|\n", line, delimiter);
        if (!line)
            {
                printf("\n");
                break;}
        if (to_be_expanded == TRUE && exec && is_delimiter(line, delimiter) == FALSE)
        {
            buffer = expand_in_heredoc(exec, line);
            printf("got here and to_be_expanded is TRUE\n");
        }
        else if (is_delimiter(line, delimiter) == FALSE)
        {
            printf("line is %s buff %s", line, buffer);
            buffer = ft_strdup(line);
        }
        if (buffer)
            write(w_heredoc, buffer, ft_strlen(buffer));
    }
    close(w_heredoc);
}

void here_doc(t_exec *exec, t_redir *og_redir, t_redir *here_doc, t_bool error_flag)
{
    expand_heredoc(&here_doc);
    here_doc_helper(exec , here_doc->here_doc_fd[W_HEREDOC], og_redir->file_name, here_doc->file_name);
    // to edit later
    if (error_flag == TRUE)
        close(here_doc->here_doc_fd[R_HEREDOC]);
 }
