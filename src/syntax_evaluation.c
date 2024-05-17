/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_evaluation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 05:43:18 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/17 20:10:45 by amohdi           ###   ########.fr       */
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
    static t_bool   is_saved;
    static t_bool   is_printed;
    static char     *error_message;

    if (indicator == RESET)
    {
        is_saved = FALSE;
        is_printed = FALSE;
        free(error_message);
        error_message = NULL;
    }
    else if (indicator == SAVE && is_saved == FALSE)
    {
        if (line && *line)
            (*line) += ft_strlen(*line); 
        is_saved = TRUE;
        error_message = ft_strdup(message);
    }
    else if (indicator == RETRIEVE)
        return (is_saved);
    else if (indicator == PRINT && is_printed == FALSE)
    {
        if (error_message != NULL)
        { 
            is_printed = TRUE;
            write(STDERR_FILENO, error_message, ft_strlen(error_message));
        }
    }
    return TRUE;
}

static t_eval check_command(t_token *root)
{
    if (!root->cmd || (root->cmd->args && check_args(root->cmd->args) == FAILED))
        return (FAILED);
    return PASSED;
}

void evaluate_syntax(t_token *root)
{
    if (!root)
        return ;
    if ((root->type == PIPE || root->type == OR || root->type == AND) && (!root->r_token || !root->l_token))
        ft_print_error("Syntax Error\n", NULL, SAVE);
    if (root->type == CMD && check_command(root) == FAILED)
        ft_print_error("Syntax Error\n", NULL, SAVE);
    evaluate_syntax(root->l_token);
    evaluate_syntax(root->r_token);
}
