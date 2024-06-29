/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 20:47:10 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 18:26:53 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_file_name(char **line, char **file_name, t_lvars *vars)
{
	if (is_quote((*line)[vars->len]) == TRUE)
	{
		*file_name = get_token_with_quotes(line, vars->len, &(vars->og_len));
		vars->len = 0;
	}
	else
		*file_name = ft_substr(*line, 0, vars->len);
	vars->og_len += vars->len;
	(*line) += vars->len;
}

void	check_for_expansion_in_heredoc(t_token **token, t_bool is_root,
		REDIR_MODE mode)
{
	if (is_root == FALSE)
	{
		if (mode == HEREDOC
			&& ft_check_for_quotes(
				(*token)->cmd->og_tokens->og_redir->file_name) == TRUE)
			(*token)->cmd->og_tokens->og_redir->to_be_expanded = TRUE;
	}
	else
	{
		if (mode == HEREDOC
			&& ft_check_for_quotes((*token)->og_redir->file_name) == TRUE)
			(*token)->cmd->og_tokens->og_redir->to_be_expanded = TRUE;
	}
}

t_bool	is_valid_filename(char c)
{
	if (c != '\0' && is_space(c) == FALSE && is_special_char(c) == FALSE
		&& c != ')' && c != '(')
	{
		return (TRUE);
	}
	return (FALSE);
}

int	calculate_file_name_len(char **line, t_lvars *vars)
{
	if (!**line || is_redirection_char(**line) == TRUE
		|| (is_special_char(**line) == TRUE && is_quote(**line) == FALSE))
	{
		ft_print_error("Syntax error near unexpected token 'newline'\n", line,
			SAVE);
		return (-1);
	}
	while (is_valid_filename((*line)[vars->len]) == TRUE)
		(vars->len)++;
	if ((vars->len == 0 && is_quote((*line)[vars->len]) == FALSE))
	{
		ft_print_error("Syntax error unexpected error near '>'\n", line, SAVE);
		return (-1);
	}
	return (0);
}

void	add_redirection(t_redir **redir, REDIR_MODE mode, char *file_name)
{
	t_redir	*tmp;

	if (!(*redir))
	{
		*redir = new_cmd_redir(mode, file_name);
		if (!*redir)
			return ;
	}
	else
	{
		tmp = *redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_cmd_redir(mode, file_name);
		if (!tmp->next)
			return ;
	}
}
