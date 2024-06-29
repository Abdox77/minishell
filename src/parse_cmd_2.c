/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 13:46:55 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 16:16:08 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_valid_char_to_be_processed(char c)
{
	if (c != '\0' && c != '&' && c != '(' && c != ')' && c != '|'
		&& is_space(c) == FALSE && is_redirection_char(c) == FALSE)
	{
		return (TRUE);
	}
	return (FALSE);
}

char	*get_token_with_quotes(char **line, int len, int *og_len)
{
	char	*arg;

	arg = ft_substr(*line, 0, len);
	if (!arg)
		printf("allocation failed in get_token_with_quotes\n");
	(*line) += len;
	(*og_len) += len;
	while (is_valid_char_to_be_processed(**line) == TRUE)
	{
		if (is_quote(**line) == FALSE)
			get_token_wout_quotes(line, &arg, og_len);
		else
			get_token_wquotes(line, &arg, og_len);
	}
	return (arg);
}

void	process_line_when_parenthesis(char **line, t_token **token, int len)
{
	if (len)
	{
		if (!*token)
			*token = new_token(CMD);
		(*token)->cmd->cmd = ft_substr(*line, 0, len);
		(*token)->cmd->og_tokens->og_cmd = ft_substr(*line, 0, len);
		(*line) += len;
	}
	else
		ft_print_error("Syntax error unexpected token ')'\n", line, SAVE);
}

void	process_simple_cmd(t_token **token, char **line, int len)
{
	if (!*token)
		*token = new_token(CMD);
	(*token)->cmd->cmd = ft_substr(*line, 0, len);
	(*token)->cmd->og_tokens->og_cmd = ft_substr(*line, 0, len);
	(*line) += len;
}

void	get_command(t_token **token, char **line)
{
	char	*tmp;
	t_lvars	vars;

	init_lvars(&vars);
	tmp = *line;
	special_trim(line);
	while (check_char_validation_for_cmd(line, tmp, vars.len))
		++vars.len;
	if (is_quote(tmp[vars.len]) == TRUE)
	{
		if (!*token)
			*token = new_token(CMD);
		(*token)->cmd->cmd = get_token_with_quotes(line, vars.len,
				&vars.og_len);
		(*token)->cmd->og_tokens->og_cmd = ft_substr(*line - vars.og_len, 0,
				vars.og_len);
	}
	else if (tmp[vars.len] == ')')
		process_line_when_parenthesis(line, token, vars.len);
	else if (vars.len)
		process_simple_cmd(token, line, vars.len);
}
