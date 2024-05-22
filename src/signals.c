/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:40:30 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/22 10:16:10 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sig(int sig)
{
    if (sig == SIGINT)
        write(STDOUT_FILENO, "hello\n", 6);
    // else if (sig == SIGTERM)
    //         return;
    else if (sig == SIGQUIT)
        return ;
}
static void    ft_handle_sigint(int sig)
{
    // int    *p;

    if (sig != SIGINT)
        return ;
    printf("\n");
    rl_replace_line("", 1);
    rl_on_new_line();
    rl_redisplay();
    // p = ft_global_exit_status();
    // *p = 1;
}

void singal_handler(void)
{
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, ft_handle_sigint);
}



// void    ft_signals(void)
// {
    
// }