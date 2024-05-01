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

t_bool is_an_operator(char *line)
{
	if ((*line == '&' && *(line + 1) && *(line + 1) == '&') || (*line == '|' && *(line + 1) && *(line + 1) == '|'))
		return true;
	return false;
}

// void handle_operator(t_token **token, char **line)
// {
// 	TOKEN type;
// 	t_token *tmp;

// 	printf("in handle_operator line is : %s\n", *line);
// 	if (**line == '&')
// 	{
// 		printf("type is and\n");
// 		type = AND;
// 	}
// 	else
// 		type = OR;
// 	(*line) += 2;
// 	if (*token)
// 	{
// 		if ((*token)->type == PIPE)
// 			printf("its a fucking pipe\n");
// 		tmp = *token;
// 		*token = new_token(type);
// 		(*token)->l_token = tmp;
// 		(*token)->r_token = NULL;
// 		lexer(&((*token)->r_token), line);
// 	}
// 	else
// 		ft_error("syntax error here\n", EXIT_FAILURE); // in case of syntax error just print the error
// }

static void place_cmd_node(t_token **root, t_token **to_put)
{
	if (!*root)
		*root = *to_put;
	else if ((*root)->type != CMD)
	{
		if (!((*root)->l_token))
			(*root)->l_token = *to_put;
		else if (!((*root)->r_token))
			(*root)->r_token = *to_put;
		else
			place_cmd_node(&((*root)->r_token), to_put);
	}
}

static void place_pipe_node(t_token **root, t_token **to_put)
{
	t_token *tmp;

	if (!*root)
		*root = *to_put;
	else if ((*root)->type == CMD)
	{
		tmp = *root;
		*root = *to_put;
		(*root)->l_token = tmp;
	}
	else
		place_pipe_node((&(*root)->r_token), to_put);
}

static void	place_operator_node(t_token **root, t_token **to_put)
{
	t_token *tmp;

	if (!*root)
		printf("syntax error here\n");
	else
	{
		tmp = *root;
		*root = *to_put;
		(*root)->l_token = tmp;
	}
}

static void place_node(t_token **root, t_token **to_put, TOKEN type)
{
	if (type == CMD)
	{	
		// printf("command befor placecmd %s\n", )	
		place_cmd_node(root, to_put);}
	else if (type == AND)
		place_operator_node(root, to_put);
	else if (type == PIPE)
		place_pipe_node(root, to_put);
	else
		printf("what i'm doing here lol\n");
}

static void handle_operators(t_token **token, char **line)
{
	t_token *tmp;

	tmp = new_token(AND);
	(*line) += 2;
	place_node (token, &tmp, AND);
}

static void handle_pipes(t_token **token, char **line)
{
	t_token *tmp;

	tmp = new_token(PIPE);
	(*line) += 1;
	place_node (token, &tmp, PIPE);
}

static void handle_commands(t_token **root, char **line)
{
	t_token *tmp;

	tmp = handle_command(line);
	place_node(root, &tmp, CMD);
}

void	lexer(t_token **token, char **line)
{

	if (!*line || !**line)
		return ;
	if (is_an_operator(*line) == true)
		handle_operators(token, line);
	else if (**line == '|')
		handle_pipes(token, line);
	else
		handle_commands(token, line);
		lexer(token, line);
}
