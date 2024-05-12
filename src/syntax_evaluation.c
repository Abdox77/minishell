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


static t_eval  check_args(char **args)
{
    // int i;
    // int len;

    // if (args)
    // {
    //     i = -1;
    //     len = strs_len(args);
    //     while(++i < len)
    //     {
    //         if(args[i] && !args[i][0])
    //             return FAILED;
    //     }
    // }
    (void)args;
    return PASSED;
}

static t_eval check_commands(t_token *root)
{
    if (!root->cmd || (root->cmd && !root->cmd->cmd) || (root->cmd->args && check_args(root->cmd->args) == FAILED))
        return (PASSED);
        // return (printf("Syntax Error\n"), PASSED);
    return PASSED;
}

static t_eval check_tree(t_token *root)
{
    (void)root;
    return PASSED;
}

t_eval synatx_evaluator(t_token *root)
{

    if (!root)
        return FAILED;
    else if (root && (check_tree(root) == FAILED || check_commands(root) == FAILED))
        return (FAILED);
    return PASSED;
}