/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:40:30 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/22 12:18:22 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void    ft_handle_sigint(int sig)
{
    (void)sig;
    if (sig_state(_NO_STATE_ , _RETRIEVE) == UNSET_SIGS)
        return ;
    printf("\n");
    rl_replace_line("", 1);
    rl_on_new_line();
    rl_redisplay();
}

void singal_handler(void)
{
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, ft_handle_sigint);
}
