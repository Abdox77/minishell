/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 10:06:01 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/18 20:59:29 by amohdi           ###   ########.fr       */
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

    i = 0;
    while(buffer[i] && delimiter[i])
    {  
        if (buffer[i] != delimiter[i])
            return FALSE;
        ++i;
    }
    if (buffer[i] == '\n' && !delimiter[i] && !buffer[i + 1])
        return (TRUE);
    return (FALSE);
}

static t_bool ft_check_for_quotes(char *og_delimiter)
{
    int     i;
    t_bool  to_be_expanded;
    
    i = -1;
    while(og_delimiter[++i])
        if (is_quote(og_delimiter[++i]) == TRUE)
            return TRUE;
    return FALSE;
}

static char *expand_in_heredoc(char *line)
{
    int i;
    int len;
    char *env_variable;

    i = -1;
    len = 0;
    while(line [++i])
    {
        if (line[i] == '$')
        {
            ++i;
            while(line[len] && is_space(line[len]) == FALSE && is_quote(line[len]) == FALSE && line[len] == '$')
                ++len;
            if (len)
            {
                env_variable = ft_get_expanded_version(ft_substr(line, 0, len));
            }
        }
        ++line;
    }
}

void here_doc_helper(int w_heredoc, char *og_delimiter, char *delimiter)
{
    char    *line;
    char    *buffer;
    t_bool  to_be_expanded;
    
    to_be_expanded = ft_check_for_quotes(og_delimiter);
    while(is_delimiter(buffer, delimiter) == FALSE)
    {
        printf(">");
        line = readline(YELLOW"Heredoc>"RESET_COLORS);
        if (to_be_expanded == TRUE)
            buffer = expand_in_heredoc(line);
        else
            buffer = ft_strdup(line);
        write(w_heredoc, buffer, ft_strlen(buffer));
        free(buffer);
    }
    close(w_heredoc);
}

void here_doc(t_redir **og_redir, t_redir **here_doc, t_bool error_flag)
{
    expand_heredoc(here_doc);
    here_doc_helper((*here_doc)->here_doc_fd[W_HEREDOC], here_doc);
    char buff [200];
    int bytes;
    bytes = read((*here_doc)->here_doc_fd[R_HEREDOC], buff, 20);
    buff[bytes] = '\0';
    // to edit later
    if (error_flag == TRUE)
        close((*here_doc)->here_doc_fd[R_HEREDOC]);
 }
