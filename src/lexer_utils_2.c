/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:12:42 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/17 19:44:21 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_redirection(t_redir **redir, REDIR_MODE mode, char *file_name)
{
    t_redir *tmp;

    if (!(*redir))
	{	
	    *redir = new_cmd_redir(mode, file_name);
	    if (!*redir)
            return ;
		    // ft_error("REDIR creation failed and returned null\n", EXIT_FAILURE);
	}
	else
	{
	    tmp = *redir;
        while(tmp->next)
		    tmp = tmp->next;
	    tmp->next = new_cmd_redir(mode, file_name);
	    if (!tmp->next)
            return ;
	    //    ft_error("REDIR creation failed and returned null\n", EXIT_FAILURE);
    }
}

// void handle_input(t_token **token, char **line, t_bool is_root)
// {
//     int			len;
//     int         og_len;
//     char 		*file_name;
//     REDIR_MODE 	mode;

//     len = 0;
//     og_len = 0;
//     file_name = NULL;
//     ++(*line);
//     if ((**line) == '<')
//     {
//         mode = HEREDOC;
//         ++(*line);
//     }
//     else
//         mode = INFILE; // or output to check later
//     special_trim(line);
//     if (!**line || is_redirection_char(**line) == TRUE || (is_special_char(**line) == TRUE && is_quote(**line) == FALSE))
//     {
//         printf("here\n");
//         ft_print_error("Syntax error near unexpected token 'newline'\n", line, SAVE);
//         return;
//     }
//     while((*line)[len] && is_space((*line)[len]) == FALSE && is_special_char((*line)[len]) == FALSE && (*line)[len] != ')' && (*line)[len] != '(')
//         len++;
//     if (!len && (*line)[len] == '\0')
//     {
//         ft_print_error("Syntax error unexpected error near '>'\n", NULL, SAVE);
//         return;
//     }
//     else if (is_quote((*line)[len]) == TRUE)
//     {
//         printf("file name is %s\n", file_name);
//         file_name = get_token_with_quotes(line, len, &og_len);
//         len = 0;
//     }
//     else
//         file_name =  ft_substr(*line, 0, len);
//     (*line) += len;
//     if (ft_print_error(NULL, NULL, RETRIEVE) == TRUE)
//     {
//         printf("entered here\n");
//         free(file_name);
//         file_name = NULL;
//     }
//     if (is_root == FALSE)
//     {
//         add_redirection(&((*token)->cmd->input), mode, file_name);
//         add_redirection(&((*token)->cmd->og_tokens->og_input), mode, ft_substr(*line - og_len, 0, og_len));
//         if (mode == HEREDOC && ft_check_for_quotes((*token)->cmd->og_tokens->og_input->file_name) == TRUE)
//             (*token)->cmd->og_tokens->og_input->to_be_expanded = TRUE;
//     }
//     else
//     {
//         add_redirection(&((*token)->input), mode, file_name);
//         add_redirection(&((*token)->og_input), mode, ft_substr(*line - og_len, 0, og_len));
//         if (mode == HEREDOC && ft_check_for_quotes((*token)->og_input->file_name) == TRUE)
//             (*token)->cmd->og_tokens->og_input->to_be_expanded = TRUE;
//     }
//     special_trim(line);
//     if (**line == '(') // i can check on other special characters here
//         ft_print_error ("Syntax error unexpected token near '('\n", line, SAVE);
// }


void handle_input(t_token **token, char **line, t_bool is_root)
{
    int len = 0, og_len = 0;
    char *file_name = NULL;
    REDIR_MODE mode = get_input_mode(line);
    special_trim(line);
    if (check_syntax_error(line))
        return;
    len = get_token_length(*line);
    if (is_quote((*line)[len]))
    {
        file_name = get_token_with_quotes(line, len, &og_len);
        len = 0;
    }
    else
        file_name = ft_substr(*line, 0, len);
    (*line) += len;
    handle_error(file_name, line);
    add_redirection_to_token(token, is_root, mode, file_name, *line - og_len);
}

REDIR_MODE get_input_mode(char **line)
{
    ++(*line);
    if (**line == '<')
    {
        ++(*line);
        return HEREDOC;
    }
    return INFILE;
}

int check_syntax_error(char **line)
{
    if (!**line || is_redirection_char(**line) || (is_special_char(**line) && !is_quote(**line)))
    {
        ft_print_error("Syntax error near unexpected token 'newline'\n", line, SAVE);
        return 1;
    }
    return 0;
}

int get_token_length(char *line)
{
    int len = 0;
    while (line[len] && !is_space(line[len]) && !is_special_char(line[len]) && line[len] != ')' && line[len] != '(')
        len++;
    return len;
}

void handle_error(char *file_name, char **line)
{
    if (ft_print_error(NULL, NULL, RETRIEVE))
    {
        free(file_name);
        file_name = NULL;
    }
}

void add_redirection_to_token(t_token **token, t_bool is_root, REDIR_MODE mode, char *file_name, int og_len)
{
    if (is_root == FALSE)
    {
        add_redirection(&((*token)->cmd->input), mode, file_name);
        add_redirection(&((*token)->cmd->og_tokens->og_input), mode, ft_substr(*line - og_len, 0, og_len));
        if (mode == HEREDOC && ft_check_for_quotes((*token)->cmd->og_tokens->og_input->file_name))
            (*token)->cmd->og_tokens->og_input->to_be_expanded = TRUE;
    }
    else
    {
        add_redirection(&((*token)->input), mode, file_name);
        add_redirection(&((*token)->og_input), mode, ft_substr(*line - og_len, 0, og_len));
        if (mode == HEREDOC && ft_check_for_quotes((*token)->og_input->file_name))
            (*token)->cmd->og_tokens->og_input->to_be_expanded = TRUE;
    }
    special_trim(line);
    if (**line == '(')
        ft_print_error("Syntax error unexpected token near '('\n", line, SAVE);
}

void init_lvars(t_lvars *vars)
{
    vars->len = 0;
    vars->og_len = 0;
}

void handle_output(t_token **token, char **line, t_bool is_root)
{
    t_lvars vars;
    char *file_name;
    REDIR_MODE mode;
    
    file_name = NULL;
    mode = get_output_mode(line);
    init_lvars(&vars);
    special_trim(line);
    if (!**line)
    {
        ft_print_error("Syntax error near unexpected token 'newline'\n", line, SAVE);
        return;
    }
    vars.len = get_token_length(*line);
    if (vars.len == 0 && !is_quote((*line)[vars.len]))
    {
        ft_print_error("Syntax error unexpected error near '>'\n", line, SAVE);
        return;
    }
    else if (is_quote((*line)[vars.len]))
    {
        file_name = get_token_with_quotes(line, len, &og_len);
        vars.len = 0;
    }
    else
        file_name = ft_substr(*line, 0, vars.len);
    vars.og_len += vars.len;
    (*line) += vars.len;
    add_output_redirection(token, is_root, mode, file_name, *line - vars.og_len);
}

REDIR_MODE get_output_mode(char **line)
{
    ++(*line);
    if (**line == '>')
    {
        ++(*line);
        return APPEND;
    }
    return TRUNC;
}

void add_output_redirection(t_token **token, t_bool is_root, REDIR_MODE mode, char *file_name, int og_len)
{
    if (is_root == FALSE)
    {
        add_redirection(&((*token)->cmd->output), mode, file_name);
        add_redirection(&((*token)->cmd->og_tokens->og_output), mode, ft_substr(*line - og_len, 0, og_len - 1));
    }
    else
    {
        add_redirection(&((*token)->output), mode, file_name);
        add_redirection(&((*token)->og_output), mode, ft_substr(*line - og_len, 0, og_len));
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
