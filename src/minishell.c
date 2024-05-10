/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/10 16:29:04 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int i = 0;

void display_cmd(t_token *token)
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
        if (token->cmd->pre_cmd)
        {
            printf("pre cmd redirections ===========>\n");
            t_redir *redir = token->cmd->pre_cmd;
            while(redir)
            {
                if (redir->mode == INFILE)
                    printf("\t\t\tINFILE : %s\n", redir->file_name);
               
                else if (redir->mode == HEREDOC)
                    printf("\t\t\tHEREDOC : %s\n", redir->file_name);
                else if (redir->mode == STD_OUT)
                    printf("\t\t\tSTD_OUT : %s\n", redir->file_name);
                redir = redir->next;
            }
        }
        if (token->cmd->post_cmd)
        {
            printf("post cmd redirections ===========>\n");
            t_redir *redir = token->cmd->post_cmd;
            while(redir)
            {
                if (redir->mode == TRUNC)
                    printf("\t\t\tTRUNC : %s\n", redir->file_name);
                else if (redir->mode == OUTFILE)
                    printf("\t\t\tOUTFILE : %s\n", redir->file_name);
                else if (redir->mode == APPEND)
                    printf("\t\t\tAPPEND : %s\n", redir->file_name);
                redir = redir->next;
            }
        }
    }
}

void display(t_token *token)
{
    if (!token)
        return;
    if (token->type != CMD)
    {
        if (token->type == AND || token->type == OR)
            printf("AND\n");
        if (token->type == PIPE)
            printf("pipe\n");
        printf("==============left->node==============\n");
        display(token->l_token);
        printf("==============right->node==============\n");
        display(token->r_token);
    }
    else
        display_cmd(token);
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

static t_token *lexer_manager(char **line)
{
	if (!*line || !**line)
		return NULL; 
    t_token *head;

    head = NULL;
	while (**line)
    {
        lexer(&head, line);
        special_trim(line);
    }
    if (head->type == AND)
        printf("ok\n");

    return head;
}

void    minishell_loop(char **env)
{
    char *line;
    t_token *head_tokens;
    t_exec exec;
    
    exec.env = parse_env(env);
    exec.envp = env_to_envp(&exec);
    head_tokens = NULL;
    while(42)
    {
        line = readline(GREEN"minishell$ " "\033[35m");
        if (!line)
            break;
        if (line)
		    add_history(line);
        special_trim(&line);
        head_tokens = lexer_manager( &line);
        display(head_tokens);
        execute(head_tokens, exec.envp);
        if (*line)
            printf("line is %s\n", line);
        head_tokens = NULL;
        
    }
}

int main(int ac, char **av, char **env)
{
    (void)av;
    // (void)env;
    if (ac == 1)
        minishell_loop(env);
}
