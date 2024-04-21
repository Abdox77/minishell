/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 15:14:16 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/21 06:49:51 by amohdi           ###   ########.fr       */
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
        new_args[i] = ft_strdup(args[i]);
    new_args[i] = ft_strdup(arg);
    printf("arg is %s\n", new_args[i]);
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

void  handle_command(t_token **token, char **line)
{
    int     len;

    if (!token || !*token)
        (*token) = new_token(CMD);
    len = 0;
    while (**line && is_space(**line) == true)
        ++(*line);
    while ((*line)[len] && is_special_char((*line)[len]) == false)
        ++len;
    (*token)->type = CMD;
    (*token)->cmd->cmd = ft_substr(*line, 0, len);
    (*line) += len;
    if ((**line))
    {
        while (**line && is_redirection_char(**line) == false  && **line != '|')
        {
            while(**line && *line && is_space(**line) == true)
                ++(*line);
            len = 0;
            while ((*line)[len] && is_special_char((*line)[len]) == false)
                ++len;
            if (!(*token)->cmd->args && len)
            {
                (*token)->cmd->args = malloc(sizeof(char *) * 2);
                (*token)->cmd->args[0] = ft_substr(*line, 0, len);
                (*token)->cmd->args[1] = NULL;
                (*line) += len;
            }
            else if ((*token)->cmd->args && len)
            {
                (*token)->cmd->args =  add_arg((*token)->cmd->args, ft_substr(*line, 0, len));
                (*line) += len;
            }
            else if (is_quote(**line) == true)
                handle_quotes(token, line);
        }
    }
}
