/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_evaluation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 05:43:18 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 13:09:14 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static e_eval  check_args(char **args)
// {
//     // int i;
//     // int len;

//     // if (args)
//     // {
//     //     i = -1;
//     //     len = strs_len(args);
//     //     while(++i < len)
//     //     {
//     //         if(args[i] && !args[i][0])
//     //             return (FAILED);
//     //     }
//     // }
//     (void)args;
//     return (PASSED);
// }
static void	reset_error_message(enum e_bool *is_saved, enum e_bool *is_printed,
		enum e_bool *err_here_doc, char **message)
{
	*is_saved = FALSE;
	*is_printed = FALSE;
	*err_here_doc = FALSE;
	free(*message);
	*message = NULL;
}

static void	print_error_message(char *error_message, enum e_bool *is_printed)
{
	if (error_message != NULL)
	{
		*is_printed = TRUE;
		write(STDERR_FILENO, error_message, ft_strlen(error_message));
	}
}

enum e_bool	ft_print_error(char *message, char **line, enum e_error indicator)
{
	static enum e_bool	is_saved;
	static enum e_bool	is_printed;
	static enum e_bool	err_here_doc;
	static char			*error_message;

	if (indicator == RESET)
		reset_error_message(&is_saved, &is_printed, &err_here_doc,
			&error_message);
	else if (indicator == RESET_HEREDOC)
	{
		err_here_doc = TRUE;
		is_saved = TRUE;
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
	else if (indicator == PRINT && is_printed == FALSE && err_here_doc == FALSE)
		print_error_message(error_message, &is_printed);
	return (TRUE);
}

static enum e_eval	check_command(t_token *root)
{
	if (!root->cmd)
		return (FAILED);
	return (PASSED);
}

void	evaluate_syntax(t_token *root)
{
	if (!root)
		return ;
	if ((root->type == PIPE || root->type == OR || root->type == AND)
		&& (!root->r_token || !root->l_token))
		ft_print_error("Syntax Error\n", NULL, SAVE);
	if (root->type == CMD && check_command(root) == FAILED)
		ft_print_error("Syntax Error\n", NULL, SAVE);
	evaluate_syntax(root->l_token);
	evaluate_syntax(root->r_token);
}
