/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:14:16 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/29 18:23:35 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**add_cmd_to_args(char *cmd, char **args)
{
	int		i;
	int		len;
	char	**new_args;

	if (!cmd || !args)
		return (NULL);
	i = -1;
	len = strs_len(args) + 1;
	new_args = malloc(sizeof(char *) * (len + 1));
	if (!new_args)
		return (NULL);
	new_args[++i] = ft_strdup(cmd);
	while (++i < len)
		new_args[i] = ft_strdup(args[i - 1]);
	new_args[i] = NULL;
	free_strs(args);
	args = NULL;
	return (new_args);
}

void	process_line_with_quotes(char **line, t_token **token, t_lvars *vars)
{
	if (!(*line)[vars->len])
		ft_print_error("Syntax Error unclosed quotes here\n", line, SAVE);
	if (!(*token)->cmd->args)
	{
		(*token)->cmd->args = malloc(sizeof(char *) * 2);
		(*token)->cmd->args[0] = get_token_with_quotes(line, vars->len,
				&vars->og_len);
		(*token)->cmd->args[1] = NULL;
		(*token)->cmd->og_tokens->og_args = malloc(sizeof(char *) * 2);
		(*token)->cmd->og_tokens->og_args[0] = ft_substr(*line - vars->og_len,
				0, vars->og_len);
		(*token)->cmd->og_tokens->og_args[1] = NULL;
	}
	else
	{
		(*token)->cmd->args = add_arg((*token)->cmd->args,
				get_token_with_quotes(line, vars->len, &vars->og_len));
		(*token)->cmd->og_tokens->og_args = add_arg(
				(*token)->cmd->og_tokens->og_args,
				ft_substr(*line - vars->og_len, 0, vars->og_len));
	}
	init_lvars(vars);
}

void	process_line_without_quotes(char **line, t_token **token, t_lvars *vars)
{
	if (!*line || (*line && !**line))
		return;
	if ((*token) && !(*token)->cmd->args && vars->len)
	{
		(*token)->cmd->args = malloc(sizeof(char *) * 2);
		(*token)->cmd->args[0] = ft_substr(*line, 0, vars->len);
		(*token)->cmd->args[1] = NULL;
		(*token)->cmd->og_tokens->og_args = malloc(sizeof(char *) * 2);
		(*token)->cmd->og_tokens->og_args[0] = ft_substr(*line, 0, vars->len);
		(*token)->cmd->og_tokens->og_args[1] = NULL;
		(*line) += vars->len;
		vars->len = 0;
	}
	else if ((*token) && (*token)->cmd->args && vars->len)
	{
		(*token)->cmd->args = add_arg((*token)->cmd->args, ft_substr(*line, 0,
					vars->len));
		(*token)->cmd->og_tokens->og_args = add_arg(
				(*token)->cmd->og_tokens->og_args,
				ft_substr(*line, 0, vars->len));
		(*line) += vars->len;
		vars->len = 0;
	}
}

void	process_line(char **line, t_token **token)
{
	t_lvars	vars;

	if (!**line)
		return;
	init_lvars(&vars);
	
	while (process_line_condition(line) == TRUE)
	{
		vars.len = 0;
		special_trim(line);
		if ((*token) && !(*token)->cmd->cmd)
			get_command(token, line);
		printf("line is after get_command %s\n", *line);
		if (**line && check_for_quotes(line, &vars) == TRUE)
			process_line_with_quotes(line, token, &vars);
		else if (**line && handle_parenthesis_error(line, vars.len) == TRUE)
			break ;
		else
			{
				printf("got on the else\n");
				process_line_without_quotes(line, token, &vars);}
		if (is_redirection_char(**line) == TRUE)
			handle_redirection(token, line, FALSE);
		if (!**line || (*line && **line && **line == ')'))
			break ;
	}
}

t_token	*handle_command(char **line)
{
	t_token	*token;

	token = NULL;
	special_trim(line);
	if (**line != '\0' && is_an_operator(line, 0) == FALSE && **line != '|')
	{
		get_command(&token, line);
		process_line(line, &token);
		if (token && token->cmd->cmd && token->cmd->args)
			token->cmd->args = add_cmd_to_args(token->cmd->cmd,
					token->cmd->args);
	}
	return (token);
}
