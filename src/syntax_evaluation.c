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

t_bool ft_print_error(char *message, char **line, t_error indicator)
{
    static t_bool is_printed;

    if (indicator == SET_TO_NOT_PRINTED)
        is_printed = FALSE;
    else if (indicator == RETRIEVE)
        return (is_printed);
    else if (indicator == PRINT)
    {
        if (is_printed == TRUE)
            return FALSE;
        if (line && *line)
            (*line) += ft_strlen(*line); 
        is_printed = TRUE;
        write(STDERR_FILENO, message, ft_strlen(message));
    }
    return FALSE;
}

static t_eval check_command(t_token *root)
{
    if (!root->cmd || (root->cmd && root->cmd->cmd && root->cmd->cmd[0] == '\0') || (root->cmd->args && check_args(root->cmd->args) == FAILED))
        return (FAILED);
    return PASSED;
}

void evaluate_syntax(t_token *root)
{
    if (!root)
        return ;
    if ((root->type == PIPE || root->type == OR || root->type == AND) && (!root->r_token || !root->l_token))
        ft_print_error("Syntax Error\n", NULL, PRINT);
    if (root->type == CMD && check_command(root) == FAILED)
        ft_print_error("Syntax Error\n", NULL, PRINT);
    evaluate_syntax(root->l_token);
    evaluate_syntax(root->r_token);
}
