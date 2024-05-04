/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:14:16 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/04 14:27:28 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



static t_bool is_parenthesis(char c)
{
    if (c == '(' || c == ')')
        return true;
    return false;
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

void skip_spaces(char **line)
{
    if (!*line || !**line)
        return ;
    while (**line && is_space(**line) == true)
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

static void get_cmd_between_parenthesis(t_token **token, char **line)
{
    ++(*line);
    *token = handle_command(line);
    if (**line != ')')
        printf("Syntax error unexpected token near (");
    else
        ++(*line);
}

static void get_command(t_token **token, char **line)
{
    int len;
    char *tmp;
    char quote;

    printf("got here in cmd\n");
    if (!*line || !**line)
        return;
    len = 0;
    tmp = *line;
    special_trim(line);
    if (**line == '(')
        get_cmd_between_parenthesis(token, line);
    else
    {
        while(tmp[len] && is_special_char(tmp[len]) == false && is_an_operator(tmp + len) == false && is_parenthesis(tmp[len]) == false)
            ++len;
        if (is_quote(tmp[len]) == true)
        {
            *token = new_token(CMD);
            quote = tmp[len];
            ++len;
            while(tmp[len] && tmp[len] != quote)
                ++len;
            if (!tmp[len])
                printf("syntax error unclosed quote\n");
            (*token)->cmd->cmd = special_cmd_quote(line, len, quote);
        }
        else if (tmp[len] == ')')
            printf("Syntax error unexpected token\n"); 
        else if (len)
        {
            *token = new_token(CMD);
            (*token)->cmd->cmd = ft_substr(*line, 0, len);
        }
        (*line) += len;
        if (*token && (*token)->cmd)
            printf("the command is %s and line is %s\n", (*token)->cmd->cmd, *line);
    }
}


t_token *handle_command(char **line)
{
    int len;
    char quote;
    t_bool quote_flag;
    t_token *cmd;

    
    cmd = NULL;
    quote_flag = false;
    special_trim(line);
    if(**line && is_an_operator(*line) == false && **line != '|')
    {
        get_command(&cmd, line);
        while(**line && is_an_operator(*line) == false && **line != '|')
        {
            len = 0;
            special_trim(line);
            while(**line && **line != '|' && is_an_operator(*line) == false && is_parenthesis(**line) == false)
            {
                if (is_quote(**line))
                {
                    quote_flag = true;
                    break;
                }
                len++;
            }
            if (quote_flag == true && len)
            {
                quote = (*line)[len++];
                while((*line)[len] && (*line)[len] != quote)    
                    ++len;
                if (!(*line)[len])
                {
                    printf("Syntax Error unclosed quotes here\n");
                    // okay , so i can add a flag in the struct of the command that indicates wether an erro happened in one of the commands so no need to allocate for the command , i'll only have to skip the command until i reach a new token
                }
                if (!cmd->cmd->args)
                {
                    cmd->cmd->args = malloc(sizeof(char *) * 2);
                    cmd->cmd->args[0] = get_token_with_quotes(line, len, quote);
                    cmd->cmd->args[1] = NULL;
                }
                else
                    cmd->cmd->args = add_arg(cmd->cmd->args, ft_substr(*line, 0, len));
                (*line) += len + 1;
            }
            else if ((*line)[len] == '(')
                printf("Syntax Error unexpected token near '('\n");
            if (cmd && !cmd->cmd->args && len)
            {
                cmd->cmd->args = malloc(sizeof(char *) * 2);
                cmd->cmd->args[0] = ft_substr(*line, 0, len);
                cmd->cmd->args[1] = NULL;
                (*line) += len;
            }
            else if (cmd && cmd->cmd->args && len)
            {
                cmd->cmd->args = add_arg(cmd->cmd->args, ft_substr(*line, 0, len));
                (*line) += len;
            }
        }
    }    
    return (cmd);
}


// static t_bool is_double_parenthesis(char *s)
// {
//     if (!s || !*s)
//         return false;
//     if (*s == ')' && *(s + 1) != '\0' && *(s + 1) == ')')
//         return true;
//     return false;
// }

// static void get_command(t_token **token, char **line)
// {
//     int len;
//     char quote;

//     len = 0;
//     skip_spaces(line);
//     while ((*line)[len] && is_special_char((*line)[len]) == false && is_an_operator(*line + len) == false && *(*line + len) != ')')
//         ++len;
//     if (is_quote((*line)[len]))
//     {
//         *token = new_token(CMD);
//         quote = (*line)[len];
//         len++;
//         while ((*line)[len] && (*line)[len] != quote)
//             ++len;
//         if (!(*line)[len])
//             printf("Syntax Error unclosed quote\n");
//         (*token)->cmd->cmd = get_token_with_quotes(line, len, quote);
//     }
//     else if (len)
//     {
//         *token = new_token(CMD);
//         if (is_double_parenthesis(*line + len) == true)
//             (*token)->cmd->cmd = ft_substr(*line, 0, len + 1);
//         else
//         {
//             (*token)->type = CMD;
//             (*token)->cmd->cmd = ft_substr(*line, 0, len);
//         }
//         (*line) += len;
//     }
// }

// t_token		*handle_command(char **line)
// {
//     int	len;
//     t_bool parenthesis_flag;
//     t_token *cmd;
    
//     parenthesis_flag = false;
//     cmd = new_token(CMD);
//     while (**line && is_space(**line) == true)
//         ++(*line);
//     if (**line && is_redirection_char(**line) == true)
//         handle_redirection(&cmd, line);
//     get_command(&cmd, line);
//     if ((**line))
//     {
//         while (**line && **line != '|' && is_an_operator(*line) == false)
//         {
//             while(**line && is_space(**line) == true) // i should a function that skipps spaces , it will optimize the code 
//                 ++(*line);
//             len = 0;
//             while ((*line)[len] && is_an_operator(*line + len) == false && is_special_char((*line)[len]) == false && *(*line + len) != ')') // a function that calculates the len of a word
//                 ++len;
//             printf("line is %s and len %d\n", *line ,len);
//             if (is_double_parenthesis(*line + len) == false && *(*line + len) == ')')
//             {
//                 printf("break here\n");
//                 break;
//             }
//             if (!cmd->cmd->args && len)
//             {
//                 if (is_double_parenthesis(*line + len) == false)
//                     parenthesis_flag = true;
//                 cmd->cmd->args = malloc(sizeof(char *) * 2);
//                 cmd->cmd->args[0] = ft_substr(*line, 0, len);
//                 cmd->cmd->args[1] = NULL;
//                 (*line) += len;
//             }
//             else if (cmd->cmd->args && len)
//             {
//                 cmd->cmd->args = add_arg(cmd->cmd->args, ft_substr(*line, 0, len));
//                 (*line) += len;
//             }
//             else if (is_quote(**line) == true) handle_quotes(&cmd, line);
// 	        if (is_redirection_char(**line)) handle_redirection(&cmd, line);
//             if (is_double_parenthesis(*line) || parenthesis_flag == true)
//                 break;
//         }
//     }
//     return (cmd);
// }
