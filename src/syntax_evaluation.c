/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_evaluation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 05:43:18 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/11 19:49:39 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static t_bool check_tree(t_token *root)
// {
    
// }

// static t_bool check_commands(t_token *root)
// {
//     if (!root->cmd->cmd || (root->cmd->args && check_args(root->cmd->args) == FAILED))
//         return (printf("Syntax Error\n"), TRUE);
//     return FALSE;
// }

// t_bool synatx_evaluator(t_token *root)
// {
//     if (check_tree(root) == FALSE || check_commands(root) == FALSE)
//         return (FALSE);
//     return TRUE;
// }