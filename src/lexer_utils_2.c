/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:12:42 by amohdi            #+#    #+#             */
/*   Updated: 2024/06/28 20:39:03 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//////////////


void get_file_name(char **line, char **file_name, t_lvars *vars)
{
    if (is_quote((*line)[vars->len]) == TRUE)
    {
        *file_name = get_token_with_quotes(line, vars->len, &(vars->og_len));
        vars->len = 0;
    }
    else
        *file_name = ft_substr(*line, 0, vars->len);
    vars->og_len += vars->len;
    (*line) += vars->len; 
}


REDIR_MODE get_redir_mode(char **line)
{
    REDIR_MODE mode;

    if (**line == '<')
    {
        mode = INFILE;
        ++(*line);
        if (**line == '<')
        {
            ++(*line);
            mode = HEREDOC;
        }
    }
    else
    {
        mode = TRUNC;
        ++(*line);
        if (**line == '>')
        {
            ++(*line);
            mode = APPEND;
        }
    }
    special_trim(line);
    return mode;
}
t_bool is_valid_filename(char c)
{
    if (c != '\0' && is_space(c) == FALSE
        && is_special_char(c) == FALSE && c != ')' && c != '(')
    {
        return TRUE;
    }
    return FALSE;
}

int calculate_file_name_len(char **line, t_lvars *vars)
{
    if (!**line || is_redirection_char(**line) == TRUE || (is_special_char(**line) == TRUE && is_quote(**line) == FALSE)) // may cause a problem because it wasn't in the redir output function 
    {
        ft_print_error("Syntax error near unexpected token 'newline'\n", line, SAVE);
        return -1;
    }
    while(is_valid_filename((*line)[vars->len]) == TRUE)
        (vars->len)++;
    printf("vars->len is %d %s and |%c|\n", vars->len, *line, (*line)[vars->len]);
    if ((vars->len == 0 && is_quote((*line)[vars->len]) == FALSE))
    {
        ft_print_error("Syntax error unexpected error near '>'\n", line, SAVE);
        return -1;
    }
    return 0;
}


/////

void add_redirection(t_redir **redir, REDIR_MODE mode, char *file_name)
{
    t_redir *tmp;

    if (!(*redir))
	{	
	    *redir = new_cmd_redir(mode, file_name);
	    if (!*redir)
            return ;
	}
	else
	{
	    tmp = *redir;
        while(tmp->next)
		    tmp = tmp->next;
	    tmp->next = new_cmd_redir(mode, file_name);
	    if (!tmp->next)
            return ;
    }
}

void check_for_expansion_in_heredoc(t_token **token , t_bool is_root, REDIR_MODE mode)
{
    if (is_root == FALSE)
    { 
        if (mode == HEREDOC && ft_check_for_quotes((*token)->cmd->og_tokens->og_input->file_name) == TRUE)
            (*token)->cmd->og_tokens->og_input->to_be_expanded = TRUE;
    }
    else
    {
        if (mode == HEREDOC && ft_check_for_quotes((*token)->og_input->file_name) == TRUE)
            (*token)->cmd->og_tokens->og_input->to_be_expanded = TRUE;
    }
}
void handle_input(t_token **token, char **line, t_bool is_root)
{
    char        *file_name;
    t_lvars     vars;
    REDIR_MODE  mode;

    init_lvars(&vars);
    file_name = NULL;
    mode = get_redir_mode(line);
    if (calculate_file_name_len(line, &vars) == -1)
        return;
    get_file_name(line, &file_name, &vars);
    if (is_root == FALSE)
    {
        add_redirection(&((*token)->cmd->input), mode, file_name);
        add_redirection(&((*token)->cmd->og_tokens->og_input), mode, ft_substr(*line - vars.og_len, 0, vars.og_len));
        check_for_expansion_in_heredoc(token , is_root, mode);
    }
    else
    {
        add_redirection(&((*token)->input), mode, file_name);
        add_redirection(&((*token)->og_input), mode, ft_substr(*line - vars.og_len, 0, vars.og_len));
        check_for_expansion_in_heredoc(token , is_root, mode);
    }
}


// void handle_output(t_token **token, char **line, t_bool is_root)
// {
//     int			len;
//     int         og_len;
//     char 		*file_name;
//     REDIR_MODE 	mode;

//     len = 0;
//     og_len = 0;
//     file_name = NULL;
//     ++(*line);
//     if ((**line) == '>')
//     {
//         mode = APPEND;
//         ++(*line);
//     }
//     else
//         mode = TRUNC; // or output to check later
//     special_trim(line);
//     if (!**line)
//     {
//         ft_print_error("Syntax error near unexpected token 'newline'\n", line, SAVE);
//         return;
//     }
//     while((*line)[len] && is_space((*line)[len]) == FALSE && is_special_char((*line)[len]) == FALSE && (*line)[len] != ')' && (*line)[len] != '(')
//         len++;
//     if (len == 0 && is_quote((*line)[len]) == FALSE)
//     {
//         ft_print_error("Syntax error unexpected error near '>'\n", line, SAVE);
//         return;
//     }
//     else if (is_quote((*line)[len]) == TRUE)
//     {
//         file_name = get_token_with_quotes(line, len, &og_len);
//         len = 0;
//     }
//     else
//         file_name = ft_substr(*line, 0, len);
//     og_len += len;
//     (*line) += len;
//     printf("file_name %c %s %d %d\n", **line, file_name, len, og_len);
//     if (is_root == FALSE)
//     {
//         add_redirection(&((*token)->cmd->output), mode, file_name);
//         add_redirection(&((*token)->cmd->og_tokens->og_output), mode, ft_substr(*line - og_len, 0, og_len - 1));
//         printf("og_len : %s\n", (*token)->cmd->og_tokens->og_output->file_name);
//     }
//     else
//     {
//         add_redirection(&((*token)->output), mode, file_name);
//         add_redirection(&((*token)->og_output), mode, ft_substr(*line - og_len, 0, og_len));
//     }
// }

void handle_output(t_token **token, char **line, t_bool is_root)
{
    char        *file_name;
    t_lvars     vars;
    REDIR_MODE  mode;


    init_lvars(&vars);
    file_name = NULL;
    mode = get_redir_mode(line);
    if (calculate_file_name_len(line, &vars) == -1)
        return;
    get_file_name(line, &file_name, &vars);
    if (is_root == FALSE)
    {
        add_redirection(&((*token)->cmd->output), mode, file_name);
        add_redirection(&((*token)->cmd->og_tokens->og_output), mode, ft_substr(*line - vars.og_len, 0, vars.og_len - 1));
    }
    else
    {
        add_redirection(&((*token)->output), mode, file_name);
        add_redirection(&((*token)->og_output), mode, ft_substr(*line - vars.og_len, 0, vars.og_len));
    }
}

void handle_redirection(t_token **token, char **line, t_bool is_root)
{
    if (!*line || !**line)
        printf("WHAT HAPPENED ?\n"); // not possible for line to be null
    if (!*token)
        *token = new_token(CMD);
	else if (**line == '<')
        handle_input(token, line, is_root);
    else
        handle_output(token, line, is_root);
}
