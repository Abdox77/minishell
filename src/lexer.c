/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:22:59 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/26 23:22:07 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_pipe(t_token **token, char **line)
{
    if (*token && (*token)->r_token)
        printf("token is not null\n");
    *token = set_pipe_head();
    if (!*token)
        printf("failed to create token");
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
			return TRUE;
		if (operator == '&')
		{
			ft_print_error("Syntax error unexpected token near '&'\n", line, SAVE);
			return TRUE;
		} 
		else
		{
			while((*line)[len + 1] && is_space((*line)[len + 1]))
				len++;
			if ((*line)[len + 1] == '|' || (*line)[len + 1] == '&')
			{
				if (operator == '|')
					ft_print_error("Syntax error unexpected token near '|'\n", line, SAVE);
				else
					ft_print_error("Syntax error unexpected token near '&'\n", line, SAVE);
				return TRUE;
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
		return;
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
	ft_print_error("Syntax error unexpected token near operator\n", line, SAVE);
	return FAILED;
}

static void handle_operators(t_token **token, char **line)
{
	t_token *tmp;
	char 	operator;

	operator = **line;
	if (check_operator_syntax(line) == FAILED)
		return ;
	if (operator == '&')
		tmp = new_token(AND);
	else
		tmp = new_token(OR);
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

static void add_redirection_after_parenthsis(t_token **node, char **line)
{
	if (is_redirection_char(**line) == TRUE)
	{
		if (!*node)
			ft_print_error("Syntax error unexpected token near ')'\n", line, SAVE);
		if ((*node))
		{
			if ((*node)->type == CMD)
				handle_redirection(node, line, FALSE);
			else
				handle_redirection(node, line, TRUE);
		}
	}
	else if(is_special_char(**line) == FALSE && **line)
		ft_print_error("Syntax error unexpected token after ')'\n", line, SAVE);
}

static void handle_parenthesis(t_token **root, char **line)
{
	t_token *node;

	printf("got here in parenthesis\n");
	++(*line);
	node = NULL;
	parenthesis_lexer(&node, line);
	if (!*line || **line != ')')
	{
		ft_print_error("Syntax error unlosed parenthesis\n", line, SAVE);
		return ;
	}
	++(*line);
	special_trim(line);
	while(is_redirection_char(**line) == TRUE)
	{
		add_redirection_after_parenthsis(&node, line);
		special_trim(line);
	}
	place_node(root, &node, PARENTHESIS);
}

void	parenthesis_lexer(t_token **token, char **line)
{
	if (!*line || !**line || **line == ')')
		return ;
	special_trim(line);
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
	else if (**line == ')')
		ft_print_error("Syntax error unexpected token near ')'\n", line, SAVE);
	else if (**line == '(' || **line == ')')
		handle_parenthesis(token, line);
	else
		handle_commands(token, line);
	lexer(token, line);
}
