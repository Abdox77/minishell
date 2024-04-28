/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/24 13:59:58 by amohdi           ###   ########.fr       */
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
    printf("line is in handle pipe %s\n", *line);
    lexer(token, line);
}





/*   OLD_LEXER
void lexer(t_token **token, char **line)
{

    if (!line || !*line || !**line)
        return;
    if (**line == '|')
        handle_pipe(token, line);
    else
        handle_command(&((*token)->l_token), line);
    if (*line)
       lexer(token, line);
}
*/

t_token *cmd_token_dup(t_token *token) // maybe its dumb to duplicate every fucking commmand it will defintly 
{
	t_token *new;

	new = new_token(CMD);
	if (!new)
		ft_error("failled to create new token in cmd_token_dup\n", 1);
	new->cmd->cmd = ft_strdup(token->cmd->cmd);
	new->cmd->args = ft_dup_args(token->cmd->args);// okay here maybe add the command to the args
	new->cmd->redir = ft_dup_redir(token->cmd->redir);
	return (new);
}

void lexer(t_token **token, char **line)
{
	t_token *cmd;
	if (!*line || !**line)
		return ;
	if (**line == '|')
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
		else if (!**line)
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
