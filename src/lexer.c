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

// static t_token *dup_token(t_token *token)
// {
// 	if (!token)
// 		return NULL;
// 	else if (token->type != CMD)
// 		return (new_token(token->type));
// 	else
// 		return (cmd_token_dup(token)); 
// }

void handle_operator(t_token **token, char **line)
{
	TOKEN type;
	t_token *tmp;

	printf("in handle_operator line is : %s\n", *line);
	if (**line == '&')
	{
		printf("type is and\n");
		type = AND;
	}
	else
		type = OR;
	(*line) += 2;
	if (*token)
	{
		if ((*token)->type == PIPE)
			printf("its a fucking pipe\n");
		tmp = *token;
		*token = new_token(type);
		(*token)->l_token = tmp;
		(*token)->r_token = NULL;
		lexer(&((*token)->r_token), line);
	}
	else
		ft_error("syntax error here\n", EXIT_FAILURE); // in case of syntax error just print the error
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
		if (!*token)
		{
			*token = cmd;
			printf("line is %s and command is %s\n", *line, (*token)->cmd->cmd);
		}
		else if (*line && **line == '|' && is_an_operator(*line) == false)
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
}

