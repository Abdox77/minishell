/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:14:16 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/18 01:05:09 by amohdi           ###   ########.fr       */
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
/////////
static char *get_token_in_between_quotes(char **line, char quote, int *og_len)
{
    int len;
    char *token;

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
    fill_token(token, line, len);
    *line += len;
    *og_len += len;
    return (token);
}

void fill_token(char *token, char **line, int len)
{
    int i = -1;
    while (++i < len)
        token[i] = (*line)[i];
    token[i] = '\0';
}
///
static char *ft_special_join(char *arg, char *buff)
{
    char *joined;

    joined = NULL;
    joined = ft_strjoin(arg, buff);
    free(arg);
    return (joined);
}

// char *get_token_with_quotes(char **line, int len, int *og_len)
// {
//     char quote;
//     char *arg;
//     char *buff;

//     arg = ft_substr(*line, 0, len);
//     (*line) += len;
//     (*og_len) += len;
//     if (!arg)
//         printf("problemo LEO\n"); // to edit later
//     buff = NULL;
//     while(**line && **line != '&' && **line != '(' && **line != ')' && **line != '|' && is_space(**line) == FALSE && is_redirection_char(**line) == FALSE)
//     {
//         if (is_quote(**line) == FALSE && **line != '(' && **line != ')' && is_special_char(**line) == FALSE)
//         {
//             len = 0;
//             while ((*line)[len] && is_special_char((*line)[len]) == FALSE)
//                 len++;
//             arg = ft_special_join(arg, ft_substr(*line, 0, len));
//             *og_len += len;
//             *line += len;
//         }
//         else if (is_quote(**line) == TRUE)
//         {   
//             quote = **line;
//             buff = get_token_in_between_quotes(line, quote, og_len);
//             arg = ft_special_join(arg, buff);
//             free(buff);
//             if(**line != quote)
//                 ft_print_error("Syntax error unclosed quotes\n", line, SAVE);
//             else
//             {
//                 ++(*line);
//                 ++(*og_len);
//             }
//         }
//     }
//     return (arg);
// }

char *get_token_with_quotes(char **line, int len, int *og_len)
{
    char *arg;
    
    arg = *arg = ft_substr(*line, 0, len);
    (*line) += len;
    (*og_len) += len;
    if (!arg)
        printf("problemo LEO\n"); // to edit later
    while (**line && !is_special_condition(**line))
    {
        if (!is_quote(**line) && !is_special_char(**line))
            len = get_token_length(*line);
            arg = append_to_arg(arg, line, len, og_len);
        else if (is_quote(**line))
            arg = handle_quote(arg, line, og_len);
    }
    return (arg);
}

int is_special_condition(char c)
{
    return (c == '&' || c == '(' || c == ')' || c == '|' || is_space(c) || is_redirection_char(c));
}

int get_token_length(char *line)
{
    int len = 0;
    while (line[len] && !is_special_char(line[len]))
        len++;
    return len;
}

char *append_to_arg(char *arg, char **line, int len, int *og_len)
{
    arg = ft_special_join(arg, ft_substr(*line, 0, len));
    *og_len += len;
    *line += len;
    return arg;
}

char *handle_quote(char *arg, char **line, int *og_len)
{
    char quote = **line;
    char *buff = get_token_in_between_quotes(line, quote, og_len);
    arg = ft_special_join(arg, buff);
    free(buff);
    if (**line != quote)
        ft_print_error("Syntax error unclosed quotes\n", line, SAVE);
    else
    {
        ++(*line);
        ++(*og_len);
    }
    return arg;
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


static void get_command(t_token **token, char **line)
{
    int len;
    int og_len;
    char *tmp;

    if (!*line || !**line)
        return;
    init_vars(&len, &og_len, line, &tmp);
    if (parse_command(token, line, &len, &og_len, tmp))
        return;
    if (len)
        finalize_command(token, line, len);
}

void init_vars(int *len, int *og_len, char **line, char **tmp)
{
    *len = 0;
    *og_len = 0;
    *tmp = *line;
    special_trim(line);
}

int parse_command(t_token **token, char **line, int *len, int *og_len, char *tmp)
{
    while (tmp[*len] && !is_special_char(tmp[*len]) && !is_an_operator(line, *len)
           && !is_parenthesis(tmp[*len]))
        (*len)++;
    if (is_quote(tmp[*len]))
    {
        handle_quote_command(token, line, len, og_len);
    }
    else if (tmp[*len] == ')')
    {
        handle_parenthesis_command(token, line, len); // maybe just replace with the actual lines
        return 1;
    }
    return 0;
}

void handle_quote_command(t_token **token, char **line, int *len, int *og_len)
{
    if (!*token)
        *token = new_token(CMD);
    (*token)->cmd->cmd = get_token_with_quotes(line, *len, og_len);
    (*token)->cmd->og_tokens->og_cmd = ft_substr(*line - *og_len, 0, *og_len);
}

void handle_parenthesis_command(t_token **token, char **line, int *len)
{
    if (*len)
    {
        if (!*token)
        *token = new_token(CMD);
        (*token)->cmd->cmd = ft_substr(*line, 0, len);
        (*token)->cmd->og_tokens->og_cmd = ft_substr(*line, 0, len);
        (*line) += len;
    }
    // else
    // {
    //     // syntax error
    // }
}


///////////////////
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

// t_token *handle_command(char **line)
// {
//     int len;
//     int og_len;
//     t_bool quote_flag;
//     t_token *token;

//     og_len = 0;
//     token = NULL;
//     quote_flag = FALSE;
//     special_trim(line);
//     if(**line != '\0' && is_an_operator(line, 0) == FALSE && **line != '|')
//     {
//         get_command(&token, line);
//         while(**line != '\0' && is_an_operator(line, 0) == FALSE && **line != '|')
//         {
//             len = 0;
//             special_trim(line);
//             if (token && !token->cmd->cmd)
//                 get_command(&token, line);
//             while((*line)[len] && (*line)[len] != '|' && is_space((*line)[len]) == FALSE && is_redirection_char((*line)[len]) == FALSE && is_an_operator(line , len) == FALSE && is_parenthesis((*line)[len]) == FALSE)
//             {
//                 if (is_quote((*line)[len]) == TRUE)
//                 {
//                     quote_flag = TRUE;
//                     break;
//                 }
//                 len++;
//             }
//             if (quote_flag == TRUE)
//             {
//                 if (!(*line)[len])
//                     ft_print_error("Syntax Error unclosed quotes here\n", line, SAVE);
//                 quote_flag = FALSE;
//                 if (!token->cmd->args)
//                 {
//                     token->cmd->args = malloc(sizeof(char *) * 2);
//                     token->cmd->args[0] = get_token_with_quotes(line, len, &og_len);
//                     token->cmd->args[1] = NULL;

//                     token->cmd->og_tokens->og_args = malloc(sizeof(char *) * 2);
//                     token->cmd->og_tokens->og_args[0] = ft_substr(*line - og_len, 0, og_len);
//                     token->cmd->og_tokens->og_args[1] = NULL;
//                     og_len = 0;
//                     len = 0;
//                 }
//                 else
//                 {
//                     token->cmd->args = add_arg(token->cmd->args, get_token_with_quotes(line, len, &og_len));
//                     token->cmd->og_tokens->og_args = add_arg(token->cmd->og_tokens->og_args, ft_substr(*line - og_len, 0, og_len));
//                     og_len = 0;
//                     len = 0;
//                 }
//             }
//             else if ((*line)[len] == '(')
//             {
//                 ft_print_error("Syntax Error unexpected token '('\n", line, SAVE);
//                 break;
//             }
//             else if (token && !token->cmd->args && len)
//             {
//                 token->cmd->args = malloc(sizeof(char *) * 2);
//                 token->cmd->args[0] = ft_substr(*line, 0, len);
//                 token->cmd->args[1] = NULL;

//                 token->cmd->og_tokens->og_args = malloc(sizeof(char *) * 2);
//                 token->cmd->og_tokens->og_args[0] = ft_substr(*line, 0, len);
//                 token->cmd->og_tokens->og_args[1] = NULL;
//                 (*line) += len;
//                 len = 0;
//             }
//             else if (token && token->cmd->args && len)
//             {
//                 token->cmd->args = add_arg(token->cmd->args, ft_substr(*line, 0, len));
//                 token->cmd->og_tokens->og_args = add_arg(token->cmd->og_tokens->og_args, ft_substr(*line, 0, len));
//                 (*line) += len;
//                 len = 0;
//             }
//             if (is_redirection_char(**line) == TRUE) handle_redirection(&token, line, FALSE);
//             if (*line && **line && **line == ')')
//                 break;
//         }
//         if(token && token->cmd->cmd && token->cmd->args)
//             token->cmd->args = add_cmd_to_args(token->cmd->cmd , token->cmd->args);
//     }    
//     return (token);
// }
