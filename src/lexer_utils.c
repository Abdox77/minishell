/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:14:16 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/27 18:34:23 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void init_lvars(t_lvars *vars)
{
    vars->len = 0;
    vars->og_len = 0;
}

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
    free(args);
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

static char *get_token_in_between_quotes(char **line, char quote, int *og_len)
{
    int i;
    int len;
    char *token;

    i = -1;
    len = 0;
    if (!*line || !**line)
        return NULL;
    ++(*line);
    ++(*og_len);
    while ((*line)[len] && (*line)[len] != quote)
        len++;
    if (!(*line)[len])
    {
        if (0 == len && (*line)[len] == quote)
        {
            ++(*line);
            ++(*og_len);
        }
        return NULL;
    }
    token = malloc(sizeof(char) * len + 1);
    if(!token)
        return (NULL);
    while(++i < len)
        token[i] = (*line)[i];
    token[i] = '\0';
    *line += len;
    *og_len += len;
    return (token);
}

static char *ft_special_join(char *arg, char *buff)
{
    char *joined;

    joined = NULL;
    joined = ft_strjoin(arg, buff);
    free(arg);
    free(buff);
    return (joined);
}

char *get_token_with_quotes(char **line, int len, int *og_len)
{
    char quote;
    char *arg;
    char *buff;

    arg = ft_substr(*line, 0, len);
    (*line) += len;
    (*og_len) += len;
    if (!arg)
        printf("problemo LEO\n"); // to edit later
    buff = NULL;
    while(**line && **line != '&' && **line != '(' && **line != ')' && **line != '|' && is_space(**line) == FALSE && is_redirection_char(**line) == FALSE)
    {
        if (is_quote(**line) == FALSE && **line != '(' && **line != ')' && is_special_char(**line) == FALSE)
        {
            len = 0;
            while ((*line)[len] && is_special_char((*line)[len]) == FALSE)
                len++;
            arg = ft_special_join(arg, ft_substr(*line, 0, len));
            *og_len += len;
            *line += len;
        }
        else if (is_quote(**line) == TRUE)
        {   
            quote = **line;
            buff = get_token_in_between_quotes(line, quote, og_len);
            arg = ft_special_join(arg, buff);
            if(**line != quote)
                ft_print_error("Syntax error unclosed quotes\n", line, SAVE);
            else
            {
                ++(*line);
                ++(*og_len);
            }
        }
    }
    return (arg);
}



// static void get_command(t_token **token, char **line)
// {
//     int len;
//     int og_len;
//     char *tmp;

//     if (!*line || !**line)
//         return;
//     len = 0;
//     og_len = 0;
//     tmp = *line;
//     special_trim(line);
//     while(tmp[len] && is_special_char(tmp[len]) == FALSE && is_an_operator(line , len) == FALSE && is_parenthesis(tmp[len]) == FALSE)
//         ++len;
//     if (is_quote(tmp[len]) == TRUE)
//     {
//         if (!*token)
//             *token = new_token(CMD);
//         (*token)->cmd->cmd = get_token_with_quotes(line, len, &og_len);
//         (*token)->cmd->og_tokens->og_cmd = ft_substr(*line - og_len, 0, og_len);
//     }
//     else if (tmp[len] == ')')
//     {
//         if (len)
//         {
//             if (!*token)
//                 *token = new_token(CMD);
//             (*token)->cmd->cmd = ft_substr(*line, 0, len);
//             (*token)->cmd->og_tokens->og_cmd = ft_substr(*line, 0, len);
//             (*line) += len;
//         }
//         else
//         {
//             // syntax error 
//             return;
//         }
//     }
//     else if (len)
//     {
//         if (!*token)
//             *token = new_token(CMD);
//         (*token)->cmd->cmd = ft_substr(*line, 0, len);
//         (*token)->cmd->og_tokens->og_cmd = ft_substr(*line, 0, len);
//         (*line) += len;
//     }
// }


t_bool check_char_validation_for_cmd(char **line, char *tmp, int len)
{
    if (tmp[len] && is_special_char(tmp[len]) == FALSE && is_an_operator(line , len) == FALSE 
        && is_parenthesis(tmp[len]) == FALSE)
    {
        return TRUE;
    }
    return FALSE;
}

void process_line_when_parenthesis(char **line, t_token **token, int len)
{
    if (len)
    {
        if (!*token)
            *token = new_token(CMD);
        (*token)->cmd->cmd = ft_substr(*line, 0, len);
        (*token)->cmd->og_tokens->og_cmd = ft_substr(*line, 0, len);
        (*line) += len;
    }
    else
        ft_print_error("Syntax error unexpected token ')'\n", line, SAVE);
}

void process_simple_cmd(t_token **token, char **line, int len)
{
    if (!*token)
        *token = new_token(CMD);
    (*token)->cmd->cmd = ft_substr(*line, 0, len);
    (*token)->cmd->og_tokens->og_cmd = ft_substr(*line, 0, len);
    (*line) += len;
}

static void get_command(t_token **token, char **line)
{
    char *tmp;
    t_lvars vars;

    init_lvars(&vars);
    tmp = *line;
    special_trim(line);
    while(check_char_validation_for_cmd(line, tmp, vars.len))
        ++vars.len;
    if (is_quote(tmp[vars.len]) == TRUE)
    {
        if (!*token)
            *token = new_token(CMD);
        (*token)->cmd->cmd = get_token_with_quotes(line, vars.len, &vars.og_len);
        (*token)->cmd->og_tokens->og_cmd = ft_substr(*line -  vars.og_len, 0, vars.og_len);
    }
    else if (tmp[vars.len] == ')')
        process_line_when_parenthesis(line, token, vars.len);
    else if (vars.len)
        process_simple_cmd(token, line, vars.len);
}



// norminette

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
    args = NULL;
    return (new_args);
}



t_bool arg_len_condition(char **line, int len)
{
    if ((*line)[len] && (*line)[len] != '|' && is_space((*line)[len]) == FALSE && is_redirection_char((*line)[len]) == FALSE && 
        is_an_operator(line , len) == FALSE && is_parenthesis((*line)[len]) == FALSE)
    {
        return (TRUE);
    }
    return FALSE;
}
t_bool process_line_condition(char **line)
{
    if (**line != '\0' && is_an_operator(line, 0) == FALSE && **line != '|')
        return TRUE;
    return FALSE;
}

t_bool     handle_parenthesis_error(char **line, int len)
{
    if ((*line)[len] == '(')
        return(ft_print_error("Syntax Error unexpected token '('\n", line, SAVE), TRUE);
    return FALSE;
}
t_bool check_for_quotes_condition(char **line , t_lvars *vars)
{
    if ((*line)[vars->len] && (*line)[vars->len] != '|' && is_space((*line)[vars->len]) == FALSE 
        && is_redirection_char((*line)[vars->len]) == FALSE && is_an_operator(line , vars->len) == FALSE 
        && is_parenthesis((*line)[vars->len]) == FALSE)
    {
        return TRUE;
    }
    return FALSE;
}

t_bool check_for_quotes(char **line, t_lvars *vars)
{
    while(check_for_quotes_condition(line , vars) == TRUE)
    {
        if (is_quote((*line)[vars->len]) == TRUE)
            return TRUE;
        vars->len++;
    }
    return FALSE;
}
void process_line_with_quotes(char **line, t_token **token, t_lvars *vars)
{
    if (!(*line)[vars->len])
        ft_print_error("Syntax Error unclosed quotes here\n", line, SAVE);
    if (!(*token)->cmd->args)
    {
        (*token)->cmd->args = malloc(sizeof(char *) * 2);
        (*token)->cmd->args[0] = get_token_with_quotes(line, vars->len, &vars->og_len);
        (*token)->cmd->args[1] = NULL;
        (*token)->cmd->og_tokens->og_args = malloc(sizeof(char *) * 2);
        (*token)->cmd->og_tokens->og_args[0] = ft_substr(*line - vars->og_len, 0, vars->og_len);
        (*token)->cmd->og_tokens->og_args[1] = NULL;
    }
    else
    {
        (*token)->cmd->args = add_arg((*token)->cmd->args, get_token_with_quotes(line, vars->len, &vars->og_len));
        (*token)->cmd->og_tokens->og_args = add_arg((*token)->cmd->og_tokens->og_args, 
                                            ft_substr(*line - vars->og_len, 0, vars->og_len));
    }
    init_lvars(vars);
}
void process_line_without_quotes(char **line, t_token **token, t_lvars *vars)
{
    if ((*token) && !(*token)->cmd->args && vars->len)
    {
        (*token)->cmd->args = malloc(sizeof(char *) * 2);
        (*token)->cmd->args[0] = ft_substr(*line, 0, vars->len);
        (*token)->cmd->args[1] = NULL;
        (*token)->cmd->og_tokens->og_args = malloc(sizeof(char *) * 2);
        (*token)->cmd->og_tokens->og_args[0] = ft_substr(*line, 0, vars->len);
        (*token)->cmd->og_tokens->og_args[1] = NULL;
        (*line) += vars->len;
        vars->len = 0;
    }
    else if ((*token) && (*token)->cmd->args && vars->len)
    {
        (*token)->cmd->args = add_arg((*token)->cmd->args, ft_substr(*line, 0, vars->len));
        (*token)->cmd->og_tokens->og_args = add_arg((*token)->cmd->og_tokens->og_args, ft_substr(*line, 0, vars->len));
        (*line) += vars->len;
        vars->len = 0;
    }
}

void process_line(char **line, t_token **token)
{
    t_lvars vars;

    init_lvars(&vars);
    while(process_line_condition(line) == TRUE)
    {
        vars.len = 0;
        special_trim(line);
        if ((*token) && !(*token)->cmd->cmd)
            get_command(token, line);
        if (check_for_quotes(line, &vars) == TRUE)
            process_line_with_quotes(line, token, &vars);
        else if (handle_parenthesis_error(line, vars.len) == TRUE)
            break;
        else
            process_line_without_quotes(line, token, &vars);
        if (is_redirection_char(**line) == TRUE) handle_redirection(token, line, FALSE);
        if (*line && **line && **line == ')')
            break;
    }
}

t_token *handle_command(char **line)
{
    t_token *token;

    token = NULL;
    special_trim(line);
    if(**line != '\0' && is_an_operator(line, 0) == FALSE && **line != '|')
    {
        get_command(&token, line);
        process_line(line, &token);
        if(token && token->cmd->cmd && token->cmd->args)
            token->cmd->args = add_cmd_to_args(token->cmd->cmd , token->cmd->args);
    }
    return token;
}
