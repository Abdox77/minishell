/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/04 22:55:48 by aabou-ib         ###   ########.fr       */
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

static void place_parenthesis_node(t_token **root, t_token **to_put)
{
	printf("got to place parent\n");
	if (!*root)
	{
		*root = *to_put;
		printf("entered here1 \n");
	}
	else if (!(*root)->l_token)
	{
		(*root)->l_token = *to_put;
		printf("entered here 2 %s\n", (*root)->cmd->cmd);
	}
	else if(!(*root)->r_token)
	{
		(*root)->r_token = *to_put;
		printf("entered here 3\n");
	}
	else
		place_parenthesis_node(&((*root)->r_token), to_put);
}

static void place_node(t_token **root, t_token **to_put, TOKEN type)
{
	if (type == CMD)
		place_cmd_node(root, to_put);
	else if (type == AND)
		place_operator_node(root, to_put);
	else if (type == PIPE)
		place_pipe_node(root, to_put);
	else if (type == PARENTHESIS)
		place_parenthesis_node(root, to_put);
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

static void handle_parenthesis(t_token **root, char **line)
{
	t_token *node;
	
	++(*line);
	node = NULL;
	parenthesis_lexer(&node, line);
	if (!*line || **line != ')')
		printf("syntax error\n");
	else if (**line == ')')
		++(*line);
	place_node(root, &node, PARENTHESIS);
}

void	parenthesis_lexer(t_token **token, char **line)
{
	if (!*line || !**line || **line == ')')
		return ;
	if (is_an_operator(*line) == true)
		handle_operators(token, line);
	else if (**line == '|')
		handle_pipes(token, line);
	// else if (**line == '(')
	// 	handle_parenthesis(token, line);
	else
		handle_commands(token, line);
	parenthesis_lexer(token, line);
}
void	lexer(t_token **token, char **line)
{
	if (!*line || !**line)
		return ;
	special_trim(line);
	if (is_an_operator(*line) == true)
		handle_operators(token, line);
	else if (**line == '|')
		handle_pipes(token, line);
	else if (**line == '(' || **line == ')')
		handle_parenthesis(token, line);
	else
		handle_commands(token, line);
	lexer(token, line);
}
