/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/16 21:24:34 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int i = 0;

void display_cmd(t_token *token)
{
    if (token && token->cmd)
    {   
        i++;
        printf("\t\tCMD IS =======>  : %s\n", token->cmd->cmd);
        printf("\t\tOG_CMD IS =======>  : %s\n", token->cmd->og_tokens->og_cmd);
        if (token->cmd->args)
        {
            printf("\t\t ARGS ARE =======>  : \n");
                for (int j = 0; token->cmd->args[j]; j++)
                    printf ("\t\t\targ[%d] : %s\n", j, token->cmd->args[j]);
            printf("\t\t OG_ARGS ARE =======>  : \n");
                for (int j = 0; token->cmd->args[j]; j++)
                    printf ("\t\t\targ[%d] : %s\n", j, token->cmd->og_tokens->og_args[j]);
        }
        if (token->cmd->input)
        {
            t_redir *redir = token->cmd->input;
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
        if (token->cmd->output)
        {
            t_redir *redir = token->cmd->output;
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
        if (token->type == AND)
            printf("AND\n");
        else if (token->type == OR)
            printf("OR\n");
        if (token->type == PIPE)
            printf("pipe\n");
        if (token->og_output && token->og_output->file_name)
            printf("og_output of the root node : %s\n", token->og_output->file_name);
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
    if (!head)
        ft_print_error("Syntax Error\n", line, PRINT);
    return head;
}

// void expand_heredoc_to_infiles(t_token **root)
// {
//     t_redir *tmp;

//     if (!*root)
//         return;
//     if ((*root)->type == CMD && (*root)->cmd && (*root)->cmd->input)
//     {
//         tmp = (*root)->cmd->input;
//         while(tmp)
//         {
//             if (tmp->mode == HEREDOC)
//             {
//                 tmp->fd = expand_heredoc();
//                 if (tmp->fd < 0)
//                     printf("Error in creating fd\n");
//             }
//             tmp = tmp->next;
//         }
//     }
//     expand_heredoc_to_infiles(&(*root)->l_token);
//     expand_heredoc_to_infiles(&(*root)->r_token);
// }

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
        evaluate_syntax(head_tokens);
        // expand_heredoc_to_infiles(&head_tokens);
        ft_print_error(NULL, NULL, PRINT);
        if (ft_print_error(NULL, NULL, RETRIEVE) == FALSE)
            execute(head_tokens, &exec);
        ft_print_error(NULL, NULL, RESET);
        // cleanup(head_tokens);
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
