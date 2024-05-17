/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:40:30 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/17 22:59:53 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_sig(int sig)
{
    if (sig == SIGINT)
        return;
    else if (sig == SIGTERM)
            return;
    else if (sig == SIGQUIT)
        return ;
}

void singal_handler(void)
{
    signal(SIGINT, handle_sig);
    signal(SIGTERM, handle_sig);
    signal(SIGQUIT, handle_sig);
}
