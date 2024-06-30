/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 13:10:30 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 13:10:39 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	stat_handler(int code, int flag)
{
	static int	exit_stat = 0;
	
	if (flag)
		exit_stat = code;
	return (exit_stat);
}
