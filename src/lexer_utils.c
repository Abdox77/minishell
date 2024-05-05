/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:14:16 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/05 09:13:50 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_bool is_parenthesis(char c)
{
    if (c == '(' || c == ')')
        return TRUE;
    return FALSE;
}

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

void skip_spaces(char **line)
{
    if (!*line || !**line)
        return ;
    while (**line && is_space(**line) == TRUE)
        ++(*line);
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

static char *get_token_with_quotes(char **line, int len, char quote)
{
    int i;
    char *cmd;
    t_bool quote_open;

    i = -1;
    quote_open = FALSE;
    printf("before segv line is %s and len is %d\n", *line ,len);
    while((*line)[len + 1] && is_space((*line)[len + 1]) == FALSE && is_quote((*line)[len + 1]) == FALSE && is_an_operator(*line + len + 1) == FALSE && (*line)[len + 1] != '|')
        len++;
    cmd = malloc(sizeof(char) * (len + 1));
    if (!cmd)
        ft_error("failed to allocate memory for cmd\n", EXIT_FAILURE);
    while (++i < len - 1)
    {
        if (**line == quote && quote_open == FALSE)
        {
            ++(*line);
            quote_open = TRUE;
        }
        else if (**line == quote && quote_open == TRUE)
            ++(*line);
        cmd[i] = **line;
        ++(*line);
    }
    cmd[i] = '\0';
    if (**line == quote)
        ++(*line); // check for uncloesed quote here
    return (cmd);
}

static void get_command(t_token **token, char **line)
{
    int len;
    char *tmp;
    char quote;

    if (!*line || !**line)
        return;
    else if (*token)
        return ;
    len = 0;
    tmp = *line;
    special_trim(line);
    while(tmp[len] && is_special_char(tmp[len]) == FALSE && is_an_operator(tmp + len) == FALSE && is_parenthesis(tmp[len]) == FALSE)
        ++len;
    if (is_quote(tmp[len]) == TRUE)
    {
        *token = new_token(CMD);
        quote = tmp[len];
        ++len;
        while(tmp[len] && tmp[len] != quote)
            ++len;
        if (!tmp[len])
            printf("syntax error unclosed quote\n");
        (*token)->cmd->cmd = get_token_with_quotes(line, len, quote);
    }
    else if (tmp[len] == ')')
    {
        if (len)
        {
                printf("here 2\n");
                *token = new_token(CMD);
                (*token)->cmd->cmd = ft_substr(*line, 0, len);
                (*line) += len;
        }
        else
        {
            //
            return;
        }
    }
    else if (len)
    {
        *token = new_token(CMD);
        (*token)->cmd->cmd = ft_substr(*line, 0, len);
        (*line) += len;
    }
    if (*token && (*token)->cmd)
        printf("the command is %s and line is\n", (*token)->cmd->cmd);
}


t_token *handle_command(char **line)
{
    int len;
    char quote;
    t_bool quote_flag;
    t_token *cmd;

    cmd = NULL;
    quote_flag = FALSE;
    special_trim(line);
    if(**line && is_an_operator(*line) == FALSE && **line != '|')
    {
        get_command(&cmd, line);
        while(**line && is_an_operator(*line) == FALSE && **line != '|')
        {
            len = 0;
            special_trim(line);
            while((*line)[len] && (*line)[len] != '|' && is_space((*line)[len]) == FALSE && is_redirection_char((*line)[len]) == FALSE && is_an_operator(*line + len) == FALSE && is_parenthesis((*line)[len]) == FALSE)
            {
                if (is_quote((*line)[len]))
                {
                    printf("break here\n");
                    quote_flag = TRUE;
                    quote = (*line)[len];
                    break;
                }
                len++;
            }
            if (quote_flag == TRUE)
            {
                ++len;
                printf("imma handle the quotes for the arguments\n");
                while((*line)[len] && (*line)[len] != quote)    
                    ++len;
                if (!(*line)[len])
                    printf("Syntax Error unclosed quotes here\n");
                if (!cmd->cmd->args)
                {
                    cmd->cmd->args = malloc(sizeof(char *) * 2);
                    cmd->cmd->args[0] = get_token_with_quotes(line, len, quote);
                    cmd->cmd->args[1] = NULL;
                }
                else
                {
                    cmd->cmd->args = add_arg(cmd->cmd->args, ft_substr(*line, 0, len));
                    (*line) += len;
                }
            }
            else if ((*line)[len] == '(')
            {
                printf("Syntax Error unexpected token near '(' %s\n", *line);
                break;
            }
            else if (cmd && !cmd->cmd->args && len)
            {
                cmd->cmd->args = malloc(sizeof(char *) * 2);
                cmd->cmd->args[0] = ft_substr(*line, 0, len);
                cmd->cmd->args[1] = NULL;
                (*line) += len;
                len = 0;
            }
            else if (cmd && cmd->cmd->args && len)
            {
                cmd->cmd->args = add_arg(cmd->cmd->args, ft_substr(*line, 0, len));
                (*line) += len;
                len = 0;
            }
            else if (is_redirection_char(**line)) handle_redirection(&cmd, line);
            else if ((*line)[len] == ')')
                break;
        }
    }    
    return (cmd);
}

