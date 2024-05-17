/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 10:06:01 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/17 19:41:50 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *get_rand_file_name_2(void)
{
    char        *buff;
    char        *fname;
    static int  i;
    
    buff = ft_itoa(i);
    if (buff == NULL)
        return (NULL);
    fname = ft_strjoin("HERE_DOC", buff);
    return fname;
}

static char *get_rand_file_name(void)
{
    int fd;
    int bytes;
    char *buff;
    
    fd = open("/dev/rand", O_RDWR);
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
    ((*heredoc_redir)->here_doc_fd[W_HEREDOC]) = open(fname, O_RDWR);
    ((*heredoc_redir)->here_doc_fd[R_HEREDOC]) = open(fname, O_RDWR);
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

void here_doc_helper(int w_heredoc, char *delimiter)
{
    ssize_t r_bytes;
    char    buffer[BUFFER_SIZE];

    write(STDOUT_FILENO , "> ", ft_strlen("> "));
    r_bytes = read(STDIN_FILENO, buffer, BUFFER_SIZE);
    if (r_bytes < 0)
        return;
    buffer[r_bytes] = '\0';
    while(is_delimiter(buffer, delimiter) == FALSE)
    {
        write(w_heredoc, buffer, r_bytes);
        write(STDOUT_FILENO, "> ", ft_strlen("> "));
        r_bytes = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (r_bytes < 0)
            return;
        buffer[r_bytes] = '\0';
    }
    close(w_heredoc);
}

void here_doc(t_redir **here_doc)
{
    expand_heredoc(here_doc);
    here_doc_helper((*here_doc)->here_doc_fd[W_HEREDOC], (*here_doc)->file_name);
    close((*here_doc)->here_doc_fd[R_HEREDOC]);
}
