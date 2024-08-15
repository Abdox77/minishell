/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:48:54 by amohdi            #+#    #+#             */
/*   Updated: 2024/08/15 04:50:52 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_exit_child(int sign)
{
	(void)sign;
	printf("\n");
	rl_replace_line("", 1);
	exit(120);
}

void	child_singal_handler(void)
{
	signal(SIGINT, _exit_child);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

enum e_sigstate	sig_state(enum e_sigstate state, enum e_sigops operation)
{
	static int	sig_state;

	if (operation == _SAVE)
		sig_state = state;
	else if (operation == _RETRIEVE)
		return (sig_state);
	return (SET_SIGS);
}
