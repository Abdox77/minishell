/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:12:42 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/30 13:23:53 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input(t_token **token, char **line, enum e_bool is_root)
{
	char				*file_name;
	t_lvars				vars;
	enum e_redir_mode	mode;

	init_lvars(&vars);
	file_name = NULL;
	mode = get_redir_mode(line);
	if (calculate_file_name_len(line, &vars) == -1)
		return ;
	get_file_name(line, &file_name, &vars);
	if (is_root == FALSE)
	{
		add_redirection(&((*token)->cmd->redir), mode, file_name);
		add_redirection(&((*token)->cmd->og_tokens->og_redir), mode,
			ft_substr(*line - vars.og_len, 0, vars.og_len));
		check_for_expansion_in_heredoc(token, is_root, mode);
	}
	else
	{
		add_redirection(&((*token)->redir), mode, file_name);
		add_redirection(&((*token)->og_redir), mode, ft_substr(*line
				- vars.og_len, 0, vars.og_len));
		check_for_expansion_in_heredoc(token, is_root, mode);
	}
}

void	handle_output(t_token **token, char **line, enum e_bool is_root)
{
	char				*file_name;
	t_lvars				vars;
	enum e_redir_mode	mode;

	init_lvars(&vars);
	file_name = NULL;
	mode = get_redir_mode(line);
	if (calculate_file_name_len(line, &vars) == -1)
		return ;
	get_file_name(line, &file_name, &vars);
	if (is_root == FALSE)
	{
		add_redirection(&((*token)->cmd->redir), mode, file_name);
		add_redirection(&((*token)->cmd->og_tokens->og_redir), mode,
			ft_substr(*line - vars.og_len, 0, vars.og_len - 1));
	}
	else
	{
		add_redirection(&((*token)->redir), mode, file_name);
		add_redirection(&((*token)->og_redir), mode, ft_substr(*line
				- vars.og_len, 0, vars.og_len));
	}
}

void	handle_redirection(t_token **token, char **line, enum e_bool is_root)
{
	if (!*line || !**line)
		return ;
	if (!*token)
		*token = new_token(CMD);
	else if (**line == '<')
		handle_input(token, line, is_root);
	else
		handle_output(token, line, is_root);
}
