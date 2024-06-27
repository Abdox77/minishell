/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_evaluation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 05:43:18 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/21 22:11:17 by amohdi           ###   ########.fr       */
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

// t_bool ft_print_error(char *message, char **line, t_error indicator)
// {
//     static t_bool   is_saved;
//     static t_bool   is_printed;
//     static t_bool err_here_doc;
//     static char     *error_message;

//     if (indicator == RESET)
//     {
//         is_saved = FALSE;
//         is_printed = FALSE;
//         err_here_doc = FALSE;
//         free(error_message);
//         error_message = NULL;
//     }
//     else if (indicator == RESET_HEREDOC)
//     {
//         err_here_doc = TRUE;
//         is_saved = TRUE;
//     }
//     else if (indicator == SAVE && is_saved == FALSE)
//     {
//         if (line && *line)
//             (*line) += ft_strlen(*line); 
//         is_saved = TRUE;
//         error_message = ft_strdup(message);
//     }
//     else if (indicator == RETRIEVE)
//         return (is_saved);
//     else if (indicator == PRINT && is_printed == FALSE && err_here_doc == FALSE)
//     {
//         if (error_message != NULL)
//         { 
//             is_printed = TRUE;
//             write(STDERR_FILENO, error_message, ft_strlen(error_message));
//         }
//     }
//     return TRUE;
// }

t_bool ft_print_error(char *message, char **line, t_error indicator)
{
    static t_bool is_saved;
    static t_bool is_printed;
    static t_bool err_here_doc;
    static char *error_message;

    if (indicator == RESET)
        return reset_error(&is_saved, &is_printed, &err_here_doc, &error_message);
    else if (indicator == RESET_HEREDOC)
        err_here_doc = TRUE;
    else if (indicator == SAVE && !is_saved)
        save_error(message, line, &is_saved, &error_message);
    else if (indicator == RETRIEVE)
        return is_saved;
    else if (indicator == PRINT && !is_printed && !err_here_doc)
        print_error(&is_printed, error_message);
    return TRUE;
}

t_bool reset_error(t_bool *is_saved, t_bool *is_printed,
                   t_bool *err_here_doc, char **error_message)
{
    *is_saved = FALSE;
    *is_printed = FALSE;
    *err_here_doc = FALSE;
    free(*error_message);
    *error_message = NULL;
    return TRUE;
}

void save_error(char *message, char **line, t_bool *is_saved, char **error_message)
{
    if (line && *line)
        (*line) += ft_strlen(*line);
    *is_saved = TRUE;
    *error_message = ft_strdup(message);
}

void print_error(t_bool *is_printed, char *error_message)
{
    if (error_message)
    {
        *is_printed = TRUE;
        write(STDERR_FILENO, error_message, ft_strlen(error_message));
    }
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
