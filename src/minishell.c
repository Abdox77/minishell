/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/24 20:03:28 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int i = 0;

static void display_cmd(t_token *token)
{
    printf("THIS IS A CMD NUMBER : %d\n", i);
    if (token && token->cmd)
    {   
        i++;
        printf("\t\tCMD IS =======>  : %s\n", token->cmd->cmd);
        if (token->cmd->args)
        {
            printf("\t\t ARGS ARE =======>  : \n");
                for (int j = 0; token->cmd->args[j]; j++)
                    printf ("\t\t\targ[%d] : %s\n", j, token->cmd->args[j]);
        }
        if (token->cmd->redir)
        {
            t_redir *redir = token->cmd->redir;
            while(redir)
            {
                if (redir->mode == INFILE)
                    printf("\t\t\tINFILE : %s\n", redir->file_name);
                else if (redir->mode == OUTFILE)
                    printf("\t\t\tOUTFILE : %s\n", redir->file_name);
                else if (redir->mode == HEREDOC)
                    printf("\t\t\tHEREDOC : %s\n", redir->file_name);
                else if (redir->mode == APPEND)
                    printf("\t\t\tAPPEND : %s\n", redir->file_name);
                else if (redir->mode == TRUNC)
                    printf("\t\t\tTRUNC : %s\n", redir->file_name);
                else if (redir->mode == STD_OUT)
                    printf("\t\t\tSTD_OUT : %s\n", redir->file_name);
                redir = redir->next;
            }
        }
    }
}

void display(t_token *token)
{
    if (!token)
        return;
    if (token->type == PIPE)
    {
        printf("pipe\n");
        display_cmd(token->l_token);
        display(token->r_token);
    }
        // display_pipe(token);
    else if (token->type == CMD)
        display_cmd(token);
    // display(token->r_token);
}

void special_trim(char **line)
{
    int len;
    char *new_line;

    if (!*line || !**line)
        return ;
    skip_spaces(line);
    len = ft_strlen(*line);
    if (len > 1)
    {
        while (len - 1 && is_space(*(*line + len - 1)))
            len--;
        new_line = ft_substr(*line, 0, len);
        if (!new_line)
            return ; //error
        line = &new_line;          
    }      
}

void    minishell_loop(void)
{
    char *line;
    t_token *head_tokens;
    t_token *tmp;

    head_tokens = set_pipe_head();
    if (!head_tokens)
        return; // maybe throw an error here
    while(42)
    {
        line = readline(GREEN"minishell$ " "\033[35m");
        if (!line)
            break;
        if (line)
		    add_history(line);
        tmp = head_tokens;
        special_trim(&line);
        lexer(&(tmp), &line);
        display(head_tokens);
        if (*line)
            printf("line is %s\n", line);
        else
            printf("line is empty\n");
        head_tokens = set_pipe_head();
        
    }
}

int main(int ac, char **av, char **env)
{
    (void)av;
    (void)env;
    if (ac == 1)
        minishell_loop();
}
