/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:12:42 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/16 23:20:59 by amohdi           ###   ########.fr       */
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

void handle_input(t_token **token, char **line)
{
    int			len;
    int         og_len;
    char 		*file_name;
    REDIR_MODE 	mode;

    len = 0;
    og_len = 0;
    file_name = NULL;
    ++(*line);
    if ((**line) == '<')
    {
        mode = HEREDOC;
        ++(*line);
    }
    else
        mode = INFILE; // or output to check later
    special_trim(line);
    if (!**line)
        ft_print_error("Syntax error near unexpected token 'newline'\n", line, SAVE);
    while((*line)[len] && is_space((*line)[len]) == FALSE && is_special_char((*line)[len]) == FALSE)
        len++;
    if (!len && (*line)[len] == '\0')
        ft_print_error("Syntax error unexpected error near '>'\n", NULL, SAVE);
    else if (is_quote((*line)[len]) == TRUE)
    {
        printf("file name is %s\n", file_name);
        file_name = get_token_with_quotes(line, len, &og_len);
        len = 0;
    }
    else
        file_name =  ft_substr(*line, 0, len);
    add_redirection(&((*token)->cmd->input), mode, file_name);
    add_redirection(&((*token)->cmd->og_tokens->og_input), mode, ft_substr(*line - og_len, 0, og_len));
    special_trim(line);
    if (**line == '(') // i can check on other special characters here
        ft_print_error ("Syntax error unexpected token near '('\n", line, SAVE);
}

void handle_output(t_token **token, char **line, t_bool is_root)
{
    int			len;
    int         og_len;
    char 		*file_name;
    REDIR_MODE 	mode;

    len = 0;
    og_len = 0;
    file_name = NULL;
    ++(*line);
    if ((**line) == '>')
    {
        mode = APPEND;
        ++(*line);
    }
    else
        mode = TRUNC; // or output to check later
    special_trim(line);
    if (!**line)
        ft_print_error("Syntax error near unexpected token 'newline'\n", line, SAVE);
    while((*line)[len] && is_space((*line)[len]) == FALSE && is_special_char((*line)[len]) == FALSE)
        len++;
    if (len == 0 && is_quote((*line)[len]) == FALSE)
        ft_print_error("Syntax error unexpected error near '>'\n", line, SAVE);
    else if (is_quote((*line)[len]) == TRUE)
    {
        file_name = get_token_with_quotes(line, len, &og_len);
        len = 0;
    }
    else
        file_name = ft_substr(*line, 0, len);
    og_len += len;
    (*line) += len;
    printf("file_name %s %d\n", file_name, len);
    if (is_root == FALSE)
    {
        add_redirection(&((*token)->cmd->output), mode, file_name);
        add_redirection(&((*token)->cmd->og_tokens->og_output), mode, ft_substr(*line - og_len, 0, og_len));
        printf("og_len : %s\n", (*token)->cmd->og_tokens->og_output->file_name);
    }
    else
    {
        add_redirection(&((*token)->output), mode, file_name);
        add_redirection(&((*token)->og_output), mode, ft_substr(*line - og_len, 0, og_len));
    }
}

void handle_redirection(t_token **token, char **line, t_bool is_root)
{
    if (!*line || !**line)
        ft_error("WHAT HAPPENED ?\n", EXIT_FAILURE); // not possible for line to be null
    if (!*token)
        *token = new_token(CMD);
	else if (**line == '<')
        handle_input(token, line);
    else
        handle_output(token, line, is_root);
}
