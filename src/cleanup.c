/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 01:46:03 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/14 21:01:12 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    ft_error(char *error_message, int exit_code)
{
    ft_putstr_fd(error_message, STDERR_FILENO);
    exit(exit_code);
}

void free_strs(char **strs)
{
    int i;

    i = -1;
    while(strs[++i])
        free(strs[i]);
    free(strs);
}
