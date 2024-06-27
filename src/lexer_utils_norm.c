#include "minishell.h"

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