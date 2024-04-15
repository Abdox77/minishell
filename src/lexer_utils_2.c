/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:12:42 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/15 15:17:20 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_redirection(t_token *token, REDIR_MODE mode, char *file_name)
{
	t_redir *tmp;

	if (!token->cmd->redir)
	{	
		token->cmd->redir = new_cmd_redir(mode, file_name);
		if (!token->cmd->redir)
			ft_error("REDIR creation failed and returned null\n", EXIT_FAILURE);
	}
	else
	{
		tmp = token->cmd->redir;
		while(tmp->next)
			tmp = tmp->next;
		tmp = new_cmd_redir(mode, file_name);
		if (!tmp)
			ft_error("REDIR creation failed and returned null\n", EXIT_FAILURE);				
	}
}

void handle_input(t_token *token, char **line)
{
    int 		len;
    char 		c;
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
    while ((**line) && is_space(**line) == false)
        ++(*line);
    if (is_quote(**line) == true)
    {
        c = **line;
        (*line)++;
        while ((*line)[len] && (*line)[len] != c)
            ++len;
        if (!*line[len])
            ft_error("Syntax Error unclosed quote\n", 126); // okay maybe use free here => to do later
        file_name = ft_substr(*line, 0, len);
        (*line) += len; // removed len + 1 for debubugin purposes 
    }
    else
    {
        while ((*line)[len] && is_special_char((*line)[len]) == false)
            ++len;
        file_name = ft_substr(*line, 0, len);
        (*line) += len;
    }
    add_redirection(token, mode, file_name);
    printf("hello : %s\n", token->cmd->redir->file_name);
}

void handle_output(t_token *token, char **line)
{
    int			len;
    char		c;
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
    while ((**line) && is_space(**line) == false)
        ++(*line);
    if (**line == '\'' || **line == '"')
    {
        c = **line;
        (*line)++;
        while ((*line)[len] && (*line)[len] != c)
            ++len;
        if (!*line[len])
            ft_error("Syntax Error unclosed quote\n", 126); // okay maybe use free here => to do later
        file_name = ft_substr(*line, 0, len);
        (*line) += len; // removed len + 1 for debubugin purposes 
    }
    else
    {
        while ((*line)[len] && is_special_char((*line)[len]) == false)
            ++len;
        file_name = ft_substr(*line, 0, len);
        (*line) += len;
    }
    add_redirection(token, mode, file_name);
    printf("OUTFILE\n");
}

void handle_redirection(t_token *token, char **line, REDIR_MODE type)
{
    if (!*line || !**line || !token || !token->cmd)
        ft_error("WHAT HAPPENED ?\n", EXIT_FAILURE); // not possible for line to be null
 
    if (type == INFILE)
        handle_input(token, line);
    else
        handle_output(token, line);

    t_redir *tmp;

    tmp = token->cmd->redir;
    for(;tmp->next;)
    	printf("redir to file namre : %s\n", tmp->file_name);
}
