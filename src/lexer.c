/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/12 14:03:48 by amohdi           ###   ########.fr       */
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



t_bool is_an_operator(char **line, int len)
{
	char operator;
	if (((*line)[len] == '&' || (*line)[len] == '|'))
	{
		operator = (*line)[len];
		if ((*line)[len + 1] == operator)
		{
			// printf("returned true\n");
			return TRUE;
		}
		if (operator == '&')
		{
			printf("Syntax error unexpected token near '&'\n");
			(*line) += ft_strlen(*line);
			return FALSE;
		}
		else
		{
			while((*line)[len + 1] && is_space((*line)[len + 1]))
				len++;
			if ((*line)[len + 1] == '|' || (*line)[len + 1] == '&')
			{
				printf("Syntax error unexpected token near '%c'\n", operator);
				(*line) += ft_strlen(*line);
				return FALSE;
			}
		}
	}
	return FALSE;
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
	else if (*root && (*root)->r_token && ((*root)->r_token->type == AND || (*root)->r_token->type == OR))
	{
		tmp = (*root)->r_token;
		(*root)->r_token = *to_put;
		(*root)->r_token->l_token = tmp;
	}
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
	if (!*root)
		*root = *to_put;
	else if (!(*root)->l_token)
		(*root)->l_token = *to_put;
	else if(!(*root)->r_token)
		(*root)->r_token = *to_put;
	else
		place_parenthesis_node(&((*root)->r_token), to_put);
}

static void place_node(t_token **root, t_token **to_put, TOKEN type)
{
	if (type == CMD)
		place_cmd_node(root, to_put);
	else if (type == AND || type == OR)
		place_operator_node(root, to_put);
	else if (type == PIPE)
		place_pipe_node(root, to_put);
	else if (type == PARENTHESIS)
		place_parenthesis_node(root, to_put);
	else
		printf("what i'm doing here lol\n");
}

static t_token *new_operator(char **line)
{
	if (**line == '|')
		return (new_token(OR));
	else
		return (new_token(AND));
}

static t_eval check_operator_syntax(char **line)
{
	char operator;

	operator = **line;
	++(*line);
	if (**line == operator)
	{
		++(*line);
		return (PASSED);
	}
	printf("Syntax error unexpected token near operator\n");
	(*line) += ft_strlen(*line);
	return FAILED;
}

static void handle_operators(t_token **token, char **line)
{
	t_token *tmp;

	if (check_operator_syntax(line) == FAILED)
		return ;
	tmp = new_operator(line);
	(*line) += 2;
	place_node(token, &tmp, tmp->type);
}

static void handle_pipes(t_token **token, char **line)
{
	t_token *tmp;

	tmp = new_token(PIPE);
	(*line) += 1;
	place_node(token, &tmp, PIPE);
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
	{
		set_error_message(&node, "", line);
		return ;
	}
	if (**line == ')')
		++(*line);
	place_node(root, &node, PARENTHESIS);
}

void	parenthesis_lexer(t_token **token, char **line)
{
	if (!*line || !**line || **line == ')')
		return ;
	if (is_an_operator(line, 0) == TRUE)
		handle_operators(token, line);
	else if (**line == '|')
		handle_pipes(token, line);
	else if (**line == '(')
		handle_parenthesis(token, line);
	else
		handle_commands(token, line);
	parenthesis_lexer(token, line);
}

void	lexer(t_token **token, char **line)
{
	if (!*line || !**line)
		return ;
	special_trim(line);
	if (is_an_operator(line, 0) == TRUE)
		handle_operators(token, line);
	else if (**line == '|')
		handle_pipes(token, line);
	else if (**line == '(' || **line == ')')
		handle_parenthesis(token, line);
	else
		handle_commands(token, line);
	lexer(token, line);
}
