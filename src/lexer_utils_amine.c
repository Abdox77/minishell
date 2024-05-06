/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:14:16 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/03 23:20:25 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **add_arg(char **args, char *arg)
{
    int i;
    int len;
    char **new_args;

    i = -1;
    len = strs_len(args);
    new_args = malloc(sizeof(char *) * (len + 2));
    while (args[++i])
    {
        new_args[i] = ft_strdup(args[i]);
        free(args[i]);
    }
    new_args[i] = ft_strdup(arg);
    free(arg);
    new_args[i + 1] = NULL;
    return (new_args);
}

t_cmd *new_cmd(void)
{
    t_cmd *cmd;

    cmd = malloc(sizeof(t_cmd));
    if (!cmd)
        return NULL;
    memset(cmd, 0, sizeof(t_cmd));
    return (cmd);
}

void handle_quotes(t_token **token, char ** line)
{
    char    c;
    int     len;
            
    len = 0;
    c = (**line);
    (*line)++;
    while ((*line)[len] && (*line)[len] != c)
        ++len;
    if (!(*token)->cmd->args)
    {
        (*token)->cmd->args = malloc(sizeof(char *) * 2);
        (*token)->cmd->args[0] = ft_substr((*line), 0, len);
        (*token)->cmd->args[1] = NULL;
        (*line) += len + 1;
    }
    else if (len)
    {
        (*token)->cmd->args = add_arg((*token)->cmd->args, ft_substr(*line, 0, len));
        (*line) += len + 1; 
    }
}

static char *special_cmd_quote(char **line, int len, char quote)
{
    int i;
    char *cmd;
    t_bool quote_open;

    i = -1;
    quote_open = false;
    cmd = malloc(sizeof(char) * (len + 1));
    if (!cmd)
        ft_error("failed to allocate memory for cmd\n", EXIT_FAILURE);
    while (++i < len - 1)
    {
        if (**line == quote && quote_open == false)
        {
            ++(*line);
            quote_open = true;
        }
        cmd[i] = **line;
        ++(*line);
    }
    cmd[i] = '\0';
    ++(*line); // check for uncloesed quote here
    return (cmd);
}

static t_bool is_double_parenthesis(char *s)
{
    if (!s || !*s)
        return false;
    if (*s == ')' && *(s + 1) != '\0' && *(s + 1) == ')')
        return true;
    return false;
}

static void get_command(t_token **token, char **line)
{
    int len;
    char quote;

    len = 0;
    skip_spaces(line);
    while ((*line)[len] && is_special_char((*line)[len]) == false && is_an_operator(*line + len) == false && *(*line + len) != ')')
        ++len;
    if (is_quote((*line)[len]))
    {
        quote = (*line)[len];
        len++;
        while ((*line)[len] && (*line)[len] != quote)
            ++len;
        if (!(*line)[len])
            printf("Syntax Error unclosed quote\n");
        (*token)->cmd->cmd = special_cmd_quote(line, len, quote);
    }
    else if (len)
    {
        if (is_double_parenthesis(*line + len) == true)
            (*token)->cmd->cmd = ft_substr(*line, 0, len + 1);
        else
        {
            (*token)->type = CMD;
            (*token)->cmd->cmd = ft_substr(*line, 0, len);
        }
        (*line) += len;
    }
}

t_token		*handle_command(char **line)
{
    int	len;
    t_bool parenthesis_flag;
    t_token *cmd;
    
    parenthesis_flag = false;
    cmd = new_token(CMD);
    while (**line && is_space(**line) == true)
        ++(*line);
    if (**line && is_redirection_char(**line) == true)
        handle_redirection(&cmd, line);
    get_command(&cmd, line);
    if ((**line))
    {
        while (**line && **line != '|' && is_an_operator(*line) == false)
        {
            while(**line && is_space(**line) == true) // i should a function that skipps spaces , it will optimize the code 
                ++(*line);
            len = 0;
            while ((*line)[len] && is_an_operator(*line + len) == false && is_special_char((*line)[len]) == false && *(*line + len) != ')') // a function that calculates the len of a word
                ++len;
            printf("line is %s and len %d\n", *line ,len);
            if (is_double_parenthesis(*line + len) == false && *(*line + len) == ')')
            {
                printf("break here\n");
                break;
            }
            if (!cmd->cmd->args && len)
            {
                if (is_double_parenthesis(*line + len) == false)
                    parenthesis_flag = true;
                cmd->cmd->args = malloc(sizeof(char *) * 2);
                cmd->cmd->args[0] = ft_substr(*line, 0, len);
                cmd->cmd->args[1] = NULL;
                (*line) += len;
            }
            else if (cmd->cmd->args && len)
            {
                cmd->cmd->args = add_arg(cmd->cmd->args, ft_substr(*line, 0, len));
                (*line) += len;
            }
            else if (is_quote(**line) == true) handle_quotes(&cmd, line);
	        if (is_redirection_char(**line)) handle_redirection(&cmd, line);
            if (is_double_parenthesis(*line) || parenthesis_flag == true)
                break;
        }
    }
    return (cmd);
}
