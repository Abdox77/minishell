/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/17 18:46:46 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int i = 0;

static void display_cmd(t_token *token)
{
    printf("THIS IS A CMD NUMBER : %d\n", i);
    PRINTF("\t\tCMD IS =======>  : %s\n", token->cmd->cmd);
    if (token->cmd->args)
    {
        PRINTF("\t\t ARGS ARE =======>  : \n");
            for (int j = 0; toke->cmd->args[j]; j++)
                printf ("arg[%d] : %s\n", j, token->cmd->args[j]);
    }
    /*
        to test later 

    if (token->cmd->redir)
    {
        

    }*/
}

static void display_pipe(t_token *token)
{
    ++i;
    printf("THIS IS A PIPE NUMBER : %d\n", i);
}


void display(t_token *token)
{
    if (!token)
        return;
    if (token->type == PIPE)
        display_pipe(token);
    else if (token->type == CMD)
        display_cmd(token);
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
