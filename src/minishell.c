/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/04/14 20:48:46 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void    minishell_loop(void)
{
    char *line;
    t_token *head_tokens;


    head_tokens = set_pipe_head();
    if (!head_tokens)
        return; // maybe throw an error here
    while(42)
    {
        line = readline(GREEN"minishell$ " "\033[35m");
        if (!line)
            break;
        // args = split(line, " \t");

        lexer(head_tokens , &line);
        free(head_tokens);
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
