/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:12:42 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/10 21:12:22 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_redirection(t_redir **redir, REDIR_MODE mode, char *file_name)
{
    t_redir *tmp;

    if (!((*redir)))
	{	
	    *redir = new_cmd_redir(mode, file_name);
	    if (!*redir)
		    ft_error("REDIR creation failed and returned null\n", EXIT_FAILURE);
	}
	else
	{
	    tmp = *redir;
	    while(tmp->next)
		    tmp = tmp->next;
	    tmp->next = new_cmd_redir(mode, file_name);
	    if (!tmp->next)
		    ft_error("REDIR creation failed and returned null\n", EXIT_FAILURE);				
	}
}

void handle_input(t_token **token, char **line)
{
    int 		len;
    char 		quote;
    char 		*file_name;
    REDIR_MODE 	mode;

    len = 0;
    ++(*line);
    if ((**line) == '<')
    {
        mode = HEREDOC;
        ++(*line);
    }
    else
        mode = INFILE;
    while ((**line) && is_space(**line) == TRUE)
        ++(*line);
    if (is_quote(**line) == TRUE)
    {
        quote = **line;
        (*line)++;
        while ((*line)[len] && (*line)[len] != quote)
            ++len;
        if (!*line[len])
            ft_error("Syntax Error unclosed quote\n", 126); // okay maybe use free here => to do later
        file_name = ft_substr(*line, 0, len);
        (*line) += len;
    }
    else
    {
    	while(**line && is_space(**line) == TRUE)
    		++(*line);
        while ((*line)[len] && is_special_char((*line)[len]) == FALSE)
            ++len;
        file_name = ft_substr(*line, 0, len);
        (*line) += len;
    }
    add_redirection(&((*token)->cmd->input), mode, file_name);
}

void handle_output(t_token **token, char **line)
{
    int			len;
    char		quote;
    char 		*file_name;
    REDIR_MODE 	mode;


    len = 0;
    ++(*line);
    if ((**line) == '>')
    {
        mode = APPEND;
        ++(*line);
    }
    else
        mode = TRUNC; // or output to check later
    while ((**line) && is_space(**line) == TRUE)
        ++(*line);
    if (is_quote(**line) == TRUE)
    {
        quote = **line;
        (*line)++;
        while ((*line)[len] && (*line)[len] != quote)
            ++len;
        if (!*line[len])
            ft_error("Syntax Error unclosed quote\n", 126); // okay maybe use free here => to do later
        file_name = ft_substr(*line, 0, len);
        (*line) += len; // removed len + 1 for debubugin purposes 
    }
    else
    {
        while ((*line)[len] && is_special_char((*line)[len]) == FALSE)
            ++len;
        file_name = ft_substr(*line, 0, len);
        (*line) += len;
    }
    add_redirection(&((*token)->cmd->output), mode, file_name);
}

void handle_redirection(t_token **token, char **line)
{
    if (!*line || !**line || !(*token) || !(*token)->cmd)
        ft_error("WHAT HAPPENED ?\n", EXIT_FAILURE); // not possible for line to be null
	else if (**line == '<')
        handle_input(token, line);
    else
        handle_output(token, line);
}
