/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 19:12:33 by amohdi            #+#    #+#             */
/*   Updated: 2024/08/03 07:52:47 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GNL_H
# define GNL_H

#define BUFFER_SIZE 1000

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

char	*get_next_line(int fd);
char	*fix_rest(char **joined, char **r);
char	*get_line(char *joined, char **buff);
char	*ft_substrr(char *s, unsigned int start, int len);
int	    ft_strllen(char *s);
int		check_newline(char *buff);
char	*ft_strdupp(char *s1);
char	*ft_j_strcat(char *s1, const char *p, int len);
char	*ft_sjoin(char *s1, char *s2);

#endif