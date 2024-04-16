/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/16 21:18:02 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void display(t_token *token)
{
    if (!token)
        return;
    if (token->type == PIPE)
        printf ("\t\t\t\t\t\tthis is a PIPE\t\t\t\t\t\t\n");

    else if (token->type == CMD)
    {
        for(int i = 0; i < 5; i++)
            printf("\t");
        printf("this is the command  : %s\n", token->cmd->cmd);
        printf ("args ===>             ");
        if (token->cmd->args)
        {
            for (int i = 0; token->cmd->args[i]; i++)
                   printf("\t %s", token->cmd->args[i]);
            printf("\n\n\n\n\n");
        }
        t_redir *tmp;
        tmp = token->cmd->redir;
        if (tmp)
        {   
            printf("yes tmp\n");    
            for(;tmp;)
            {
                if (tmp->mode == INFILE)
                    printf("type is infile\n");
                else if (tmp->mode == HEREDOC)
                    printf("type is HEREDOC\n");
                else if (tmp->mode == TRUNC)
                    printf("type is TRUNC\n");
                else if (tmp->mode == APPEND)
                    printf("type is APPEND\n");
                printf("redir to file namre : %s\n", tmp->file_name);
                    tmp = tmp->next;
            }
                printf("\n");
            }
        }
        else
            printf("not redirection wtf\n");
    display(token->r_token);
    display(token->l_token);
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
        tmp = head_tokens;
        lexer(&(tmp) , &line);
        display(head_tokens);
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
