/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/21 22:13:33 by amohdi           ###   ########.fr       */
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
        ft_print_error("Syntax Error\n", line, SAVE);
    return head;
}

// static void    ft_handle_sigint(int sig)
// {
//     // int    *p;

//     if (sig != SIGINT)
//         return ;
//     printf("\n");
//     rl_replace_line("", 0);
//     rl_on_new_line();
//     rl_redisplay();
//     // p = ft_global_exit_status();
//     // *p = 1;
// }

static void _exit_child(int sign)
{
    (void)sign;
    rl_replace_line("", 1);
    // rl_on_new_line();
    rl_redisplay();
    exit(120);
}

static void child_singal_handler()
{
    // rl_replace_line("", 0);
    signal(SIGINT, _exit_child);
    // signal(SIGINT, ft_handle_sigint);
}

void expand_heredoc_to_infiles(t_exec *exec, t_token **root, t_bool error_flag)
{
    t_redir *tmp;
    t_redir *tmp_og;
    
    pid_t pid = fork();
    int status;

    if (pid == 0)
    {
        child_singal_handler();
        if (!*root)
            return;
        if ((*root)->type == CMD && (*root)->cmd && (*root)->cmd->input)
        {
            tmp = (*root)->cmd->input;
            tmp_og = (*root)->cmd->og_tokens->og_input;
            while(tmp)
            {
                if (tmp->mode == HEREDOC)
                    here_doc(exec , tmp_og, tmp, error_flag);     
                tmp = tmp->next;
                tmp_og = tmp_og->next;
            }
        }
        expand_heredoc_to_infiles(exec , &(*root)->l_token, error_flag);
        expand_heredoc_to_infiles(exec , &(*root)->r_token, error_flag);
        exit (0);
    }
    waitpid(pid, &status, WUNTRACED);
    if (WEXITSTATUS(status) == 120)
        ft_print_error(NULL, NULL, RESET_HEREDOC);
    // printf("%d\n", WEXITSTATUS(status));
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
        singal_handler();
        line = readline(GREEN"minishell$ " "\033[35m");
        if (!line)
            break;
        if (line)
		    add_history(line);
        special_trim(&line);
        head_tokens = lexer_manager( &line);
        display(head_tokens);
        evaluate_syntax(head_tokens);
        if (ft_print_error(NULL, NULL, RETRIEVE) == TRUE)
            expand_heredoc_to_infiles(&exec, &head_tokens, TRUE);
        ft_print_error(NULL, NULL, PRINT);
        if (ft_print_error(NULL, NULL, RETRIEVE) == FALSE)
            execute(head_tokens, &exec);
        ft_print_error(NULL, NULL, RESET);
        if (*line)
            printf("WTF LINE ISN'T EMPTY : line is %s\n", line);
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
