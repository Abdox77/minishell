#include "minishell.h"

void process_line(char **line, t_lvars *vars, t_bool *quote_flag, t_token **token)
{
    while ((*line)[vars->len] && (*line)[vars->len] != '|' && !is_space((*line)[vars->len])
           && !is_redirection_char((*line)[vars->len]) && !is_an_operator(line, vars->len)
           && !is_parenthesis((*line)[vars->len]))
    {
        if (is_quote((*line)[vars->len]))
        {
            *quote_flag = TRUE;
            break;
        }
        (vars->len)++;
    }
    handle_quote(line, vars, quote_flag, token);
    handle_parenthesis(line, vars->len);
    handle_args(line, &(vars->len), token);
    if (is_redirection_char(**line))
        handle_redirection(token, line, FALSE);
}

void handle_quote(char **line, t_lvars *vars, t_bool *quote_flag, t_token **token)
{
    if (*quote_flag)
    {
        if (!(*line)[vars->len])
            ft_print_error("Syntax Error unclosed quotes here\n", line, SAVE);
        *quote_flag = FALSE;
        if (!(*token)->cmd->args)
            init_args(line, vars->len, vars->og_len, token);
        else
            add_to_args(line, vars->len, vars->og_len, token);
    }
}

void handle_parenthesis(char **line, int *len)
{
    if ((*line)[*len] == '(')
    {
        ft_print_error("Syntax Error unexpected token '('\n", line, SAVE);
    }
}

void handle_args(char **line, int *len, t_token **token)
{
    if (*token && !(*token)->cmd->args && *len)
    {
        (*token)->cmd->args = malloc(sizeof(char *) * 2);
        (*token)->cmd->args[0] = ft_substr(*line, 0, *len);
        (*token)->cmd->args[1] = NULL;
        (*token)->cmd->og_tokens->og_args = malloc(sizeof(char *) * 2);
        (*token)->cmd->og_tokens->og_args[0] = ft_substr(*line, 0, *len);
        (*token)->cmd->og_tokens->og_args[1] = NULL;
        (*line) += *len;
        *len = 0;
    }
    else if (*token && (*token)->cmd->args && *len)
    {
        (*token)->cmd->args = add_arg((*token)->cmd->args, ft_substr(*line, 0, *len));
        (*token)->cmd->og_tokens->og_args = add_arg((*token)->cmd->og_tokens->og_args, ft_substr(*line, 0, *len));
        (*line) += *len;
        *len = 0;
    }
}

void init_args(char **line, int *len, int *og_len, t_token **token)
{
    (*token)->cmd->args = malloc(sizeof(char *) * 2);
    (*token)->cmd->args[0] = get_token_with_quotes(line, *len, og_len);
    (*token)->cmd->args[1] = NULL;
    (*token)->cmd->og_tokens->og_args = malloc(sizeof(char *) * 2);
    (*token)->cmd->og_tokens->og_args[0] = ft_substr(*line - *og_len, 0, *og_len);
    (*token)->cmd->og_tokens->og_args[1] = NULL;
    *og_len = 0;
    *len = 0;
}

void add_to_args(char **line, int *len, int *og_len, t_token **token)
{
    (*token)->cmd->args = add_arg((*token)->cmd->args, get_token_with_quotes(line, *len, og_len));
    (*token)->cmd->og_tokens->og_args = add_arg((*token)->cmd->og_tokens->og_args, ft_substr(*line - *og_len, 0, *og_len));
    *og_len = 0;
    *len = 0;
}

t_token *handle_command(char **line)
{
    t_lvars vars;
    t_bool quote_flag;
    t_token *token;

    vars.og_len = 0;
    token = NULL;
    quote_flag = FALSE;
    special_trim(line);
    if (**line != '\0' && !is_an_operator(line, 0) && **line != '|')
    {
        get_command(&token, line);
        while (**line != '\0' && !is_an_operator(line, 0) && **line != '|')
        {
            vars.len = 0;
            special_trim(line);
            if (token && !token->cmd->cmd)
                get_command(&token, line);
            process_line(line, &vars, &quote_flag, &token);
            if (*line && **line == ')')
                break;
        }
        if (token && token->cmd->cmd && token->cmd->args)
            token->cmd->args = add_cmd_to_args(token->cmd->cmd, token->cmd->args);
    }
    return (token);
}