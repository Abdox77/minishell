/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 10:06:01 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/13 15:15:04 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *generate_file_name(int heredoc_num)
{
    int     i;
    int     len;
    char    *fname;
    
    i = 0;
    len = ft_strlen(HEREDOC_LOCATION);
    fname = malloc (sizeof(char) * (len + ft_strlen("")));
}

int expand_heredoc(void)
{
    int fd;
    char *fname;
    static int heredocnum;

    fname = generate_file_name(heredocnum);
    if (!fname) 
        return (-1);
    fd = open(fname, O_RDWR);
    if (fd < 0)
        return (-1);
    unlink(fname);
    free(fname);
    heredocnum++;
    return fd;
}

static t_bool is_delimiter(char *buffer, char *delimiter)
{
    int i;

    i = 0;
    while(buffer[i] && delimiter[i])
    {  
        if (buffer[i] != delimiter[i])
            return FALSE;
    }
    if (!buffer[i] && delimiter[i] == '\n' && !delimiter[i + 1])
        return (FALSE);
    return (TRUE);
}

void here_doc(int fd, char *delimiter)
{
    ssize_t r_bytes;
    char    buffer[BUFFER_SIZE];

    write(STDOUT_FILENO, "> ", ft_strlen("> "));
    r_bytes = read(STDIN_FILENO, buffer, BUFFER_SIZE);
    if (r_bytes < 0)
        return;
    buffer[r_bytes] = '\0';
    while(is_delimiter(buffer, delimiter) == FALSE)
    {
        write(fd, buffer, r_bytes);
        write(STDOUT_FILENO, "> ", ft_strlen("> "));
        r_bytes = read(STDIN_FILENO, buffer, BUFFER_SIZE);
        if (r_bytes < 0)
        return;
        buffer[r_bytes] = '\0';
    }
}
