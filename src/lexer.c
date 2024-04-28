/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/28 22:39:54 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_pipe(t_token **token, char **line)
{
    if (*token && (*token)->r_token)
        printf("token is not null\n");
    *token = set_pipe_head();
    if (!*token)
        ft_error("failed to create token", EXIT_FAILURE);
    ++(*line);
    lexer(token, line);
}

t_token *cmd_token_dup(t_token *token) // maybe its dumb to duplicate every fucking commmand it will defintly 
{
	t_token *new;

	if (!token)
		return NULL;
	new = new_token(CMD);
	if (!new)
		ft_error("failled to create new token in cmd_token_dup\n", 1);
	new->cmd->cmd = ft_strdup(token->cmd->cmd);
	new->cmd->args = ft_dup_args(token->cmd->args);// okay here maybe add the command to the args
	new->cmd->redir = ft_dup_redir(token->cmd->redir);
	return (new);
}

t_bool is_an_operator(char *line)
{
	if ((*line == '&' && *(line + 1) && *(line + 1) == '&') || (*line == '|' && *(line + 1) && *(line + 1) == '|'))
		return true;
	return false;
}

void handle_operator(t_token **token, char **line)
{
	TOKEN type;
	t_token *tmp;
	t_token *new;

	printf("in handle_operator line %s\n", *line);
	(*line) += 2;
	if (**line == '&')
		type = AND;
	else
		type = OR;
	new = new_token(type);
	if (!new)
		ft_error("failed to create new token in handle_operator\n", EXIT_FAILURE);
	if (*token)
	{
		tmp = *token;
		token = &new;
		(*token)->l_token = tmp;
	}
	else
		ft_error("syntax error here\n", EXIT_FAILURE); // in case of syntax error just print the error
	lexer(token, line);
}

void lexer(t_token **token, char **line)
{
	t_token *cmd;
	if (!*line || !**line)
		return ;
	if (is_an_operator(*line) == true)
		handle_operator(token, line);
	else if (**line == '|')
		handle_pipe(token, line);
	else
	{
		cmd = handle_command(line);
		if (*line && **line == '|')
		{
			(*token)->l_token = cmd_token_dup(cmd);
			lexer(&((*token)->r_token), line);
			return ;
		}
		else if (!**line || (**line && is_an_operator(*line) == true))
		{
			if ((*token)->l_token)
				(*token)->r_token = cmd_token_dup(cmd);
			else
				(*token)->l_token = cmd_token_dup(cmd);
		}
		else
			printf("line is not empty how tf this function is finished and line is %s\n", *line);
	}
	if (*line)
		lexer(token, line);
}
