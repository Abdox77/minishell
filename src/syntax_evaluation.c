/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_evaluation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 05:43:18 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/05 09:12:33 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool synatx_evaluator(t_token *root)
{
    if (check_redirections(root) == FALSE || check_commands(root) == FALSE || check_pipes_placements(root) == FALSE || check_operators_placements(root) == FALSE)
        return (FALSE);
    return TRUE;
}