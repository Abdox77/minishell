/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:14:16 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/12 07:27:29 by codespace        ###   ########.fr       */
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

static char *get_token_in_between_quotes(char **line, char quote)
{
    int i;
    int len;
    char *token;

    i = -1;
    len = 0;
    if (!*line || !**line)
        return NULL;
    ++(*line);
    while ((*line)[len] && (*line)[len] != quote)
        len++;
    if (!(*line)[len] || 0 == len)
    {
        if (0 == len && (*line)[len] == quote)
            ++(*line);
        return NULL;
    }
    token = malloc(sizeof(char) * len + 1);
    if(!token)
        return (NULL);
    while(++i < len)
        token[i] = (*line)[i];
    token[i] = '\0';
    *line += len;
    return (token);
}

static char *get_token_with_quotes(char **line, int len)
{
    char quote;
    char *arg;
    char *buff;

    arg = ft_substr(*line, 0, len);
    (*line) += len;
    if (!arg)
        printf("problemo LEO\n");
    buff = NULL;
    while(**line && (**line == '"' || **line == '\''))
    {
        quote = **line;
        buff = get_token_in_between_quotes(line, quote);
        arg = ft_strjoin(arg, buff);
        if(buff)
            ++(*line);
    }
    return (arg);
}

static void get_command(t_token **token, char **line)
{
    int len;
    char *tmp;

    if (!*line || !**line)
        return;
    len = 0;
    tmp = *line;
    special_trim(line);
    while(tmp[len] && is_special_char(tmp[len]) == FALSE && is_an_operator(tmp + len) == FALSE && is_parenthesis(tmp[len]) == FALSE)
        ++len;
    if (is_quote(tmp[len]) == TRUE)
    {
        if (!*token)
            *token = new_token(CMD);
        (*token)->cmd->cmd = get_token_with_quotes(line, len);
    }
    else if (tmp[len] == ')')
    {
        if (len)
        {
            if (!*token)
                *token = new_token(CMD);
            (*token)->cmd->cmd = ft_substr(*line, 0, len);
            (*line) += len;
        }
        else
        {
            // syntax error 
            return;
        }
    }
    else if (len)
    {
        if (!*token)
            *token = new_token(CMD);
        (*token)->cmd->cmd = ft_substr(*line, 0, len);
        (*line) += len;
    }
}

static char **add_cmd_to_args(char *cmd , char **args)
{
    int i;
    int len;
    char **new_args;

    if (!cmd || !args)
        return NULL;
    i = -1;
    len = strs_len(args) + 1;
    new_args = malloc(sizeof(char *) * (len + 1));
    if (!new_args)
        return (NULL);
    new_args[++i] = ft_strdup(cmd);
    while(++i < len)
        new_args[i] = ft_strdup(args[i - 1]);
    new_args[i] = NULL;
    free_strs(args);
    return (new_args);
}

void set_error_message(t_token **token , char *message, char **line)
{
    printf("%s", message);
    if (*token)
        (*token)->error.message = message;
    if (**line)
        (*line) += ft_strlen(*line);
}

t_token *handle_command(char **line)
{
    int len;
    t_bool quote_flag;
    t_token *token;

    token = NULL;
    quote_flag = FALSE;
    special_trim(line);
    if(**line != '\0' && is_an_operator(*line) == FALSE && **line != '|')
    {
        get_command(&token, line);
        while(**line != '\0' && is_an_operator(*line) == FALSE && **line != '|')
        {
            len = 0;
            special_trim(line);
            if (token && !token->cmd->cmd)
                get_command(&token, line);
            while((*line)[len] && (*line)[len] != '|' && is_space((*line)[len]) == FALSE && is_redirection_char((*line)[len]) == FALSE && is_an_operator((*line)[len]) == FALSE && is_parenthesis((*line)[len]) == FALSE)
            {
                if (is_quote((*line)[len]) == TRUE)
                {
                    quote_flag = TRUE;
                    break;
                }
                len++;
            }
            if (quote_flag == TRUE)
            {
                if (!(*line)[len])
                    printf("Syntax Error unclosed quotes here\n");
                quote_flag = FALSE;
                if (!token->cmd->args)
                {
                    token->cmd->args = malloc(sizeof(char *) * 2);
                    token->cmd->args[0] = get_token_with_quotes(line, len);
                    token->cmd->args[1] = NULL;
                    len = 0;
                }
                else
                {
                    token->cmd->args = add_arg(token->cmd->args, get_token_with_quotes(line, len));
                    len = 0;
                }
            }
            else if ((*line)[len] == '(')
            {
                set_error_message(&token ,"Syntax Error unexpected token '('\n", line);
                break;
            }
            else if (token && !token->cmd->args && len)
            {
                token->cmd->args = malloc(sizeof(char *) * 2);
                token->cmd->args[0] = ft_substr(*line, 0, len);
                token->cmd->args[1] = NULL;
                (*line) += len;
                len = 0;
            }
            else if (token && token->cmd->args && len)
            {
                token->cmd->args = add_arg(token->cmd->args, ft_substr(*line, 0, len));
                (*line) += len;
                len = 0;
            }
            if (is_redirection_char(**line) == TRUE) handle_redirection(&token, line);
            if (*line && **line && **line == ')')
                break;
        }
        if(token && token->cmd->cmd && token->cmd->args)
            token->cmd->args = add_cmd_to_args(token->cmd->cmd , token->cmd->args);
    }    
    return (token);
}

