/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:40:30 by amohdi            #+#    #+#             */
/*   Updated: 2024/07/02 19:18:57 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_handle_sigint(int sig)
{
	(void)sig;
	if (sig_state(_NO_STATE_, _RETRIEVE) == UNSET_SIGS)
		return ;
	printf("\n");
	stat_handler(130, 1);
	rl_replace_line("", 1);
	rl_on_new_line();
	rl_redisplay();
}

static void	ft_sigint(int sig)
{
	(void)sig;
	printf("\n");
}

void	singal_handler(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_handle_sigint);
	signal(SIGTSTP, SIG_IGN);
}

void	handle_signals(void)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
}

void	handle_signals_before(int flag)
{
	if (flag == 1)
		signal(SIGINT, ft_sigint);
	else
		signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
