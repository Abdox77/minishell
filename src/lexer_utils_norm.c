#include "minishell.h"

void init_lvars(t_lvars *vars)
{
    vars->len = 0;
    vars->og_len = 0;
}

REDIR_MODE get_redir_mode(char **line)
{
    REDIR_MODE mode;

    if (**line == '<')
    {
        mode = INFILE;
        ++(**line);
        if (**line == '<')
        {
            ++(**line);
            mode = HEREDOC;
        }
    }
    else
    {
        mode = TRUNC;
        ++(**line);
        if (**line == '>')
        {
            ++(**line);
            mode = HEREDOC;
        }
    }
    special_trim(line);
    return mode;
}
t_bool is_valid_filename(char c)
{
    if (c != '\0' && is_space(c) == FALSE
        && is_special_char(c) == FALSE && (c != ')' && c != '('))
    {
        return TRUE;
    }
    return FALSE;
}

int calculate_file_name_len(char **line, t_lvars *vars)
{
    if (!**line)
    {
        ft_print_error("Syntax error near unexpected token 'newline'\n", line, SAVE);
        return;
    }
    while(is_valid_filename((*line)[vars->len]) == TRUE)
        vars->len++;
    if ((vars->len == 0 && is_quote((*line)[vars->len]) == FALSE) || !(*line)[vars->len])
    {
        ft_print_error("Syntax error unexpected error near '>'\n", line, SAVE);
        return -1;
    }
    return 0;
}

char *get_file_name(char **line, char **file_name, t_lvars *vars)
{
    if (is_quote((*line)[vars->len]) == TRUE)
    {
        *file_name = get_token_with_quotes(line, vars->len, &vars->og_len);
        vars->len = 0;
    }
    else
        *file_name = ft_substr(*line, 0, vars->len);
    // vars->og_len += vars->len;
    (*line) += vars->len; 
}

void handle_input(t_token **token, char **line, t_bool is_root)
{
    char        *file_name;
    t_lvars     vars;
    REDIR_MODE  mode;


    inti_lvars(&vars);
    file_name = NULL;
    mode = get_redir_mode(line);
    if (calculate_file_name_len(line, &vars) == -1)
        return;
    get_file_name(line, &file_name, &vars);
    if (is_root == FALSE)
    {
        add_redirection(&((*token)->cmd->input), mode, file_name);
        add_redirection(&((*token)->cmd->og_tokens->og_input), mode, ft_substr(*line - vars.og_len, 0, vars.og_len));
        if (mode == HEREDOC && ft_check_for_quotes((*token)->cmd->og_tokens->og_input->file_name) == TRUE)
            (*token)->cmd->og_tokens->og_input->to_be_expanded = TRUE;
    }
    else
    {
        add_redirection(&((*token)->input), mode, file_name);
        add_redirection(&((*token)->og_input), mode, ft_substr(*line - vars.og_len, 0, vars.og_len));
        if (mode == HEREDOC && ft_check_for_quotes((*token)->og_input->file_name) == TRUE)
            (*token)->cmd->og_tokens->og_input->to_be_expanded = TRUE;
    }
}