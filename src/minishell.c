/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/05/22 12:20:03 by amohdi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int i = 0;

void display_cmd(t_token *token)
{
    if (token && token->cmd)
    {   
        if (token->output && token->output->file_name)
            printf("og_output of the root node : %s\n", token->output->file_name);
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
            printf("output : %s og_output : %s\n", token->output->file_name, token->og_output->file_name);
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
    if (!*line || !**line)
        return ;
    while (**line && is_space(**line) == TRUE)
        ++(*line);
}

static t_token *lexer_manager(char **line)
{
	if (!*line || !**line)
		return NULL; 
    t_token *head;

    head = NULL;
    lexer(&head, line);
    if (!head)
        ft_print_error("Syntax Error\n", line, SAVE);
    return head;
}

static void _exit_child(int sign)
{
    (void)sign;
    printf("\n");
    rl_replace_line("", 1);
    exit(120);
}

static void child_singal_handler()
{
    signal(SIGINT, _exit_child);
    signal(SIGQUIT, SIG_IGN);
}

t_sigstate sig_state(t_sigstate state, t_sigops operation)
{
    static int sig_state;
    
    if (operation == _SAVE)
        sig_state = state;
    else if (operation == _RETRIEVE)
        return (sig_state);
    return SET_SIGS;
}

void _error_expand_heredoc_to_infiles(t_exec *exec, t_token **root, t_bool error_flag)
{
    int     status;
    t_redir *tmp;
    t_redir *tmp_og;
    
    if (!*root)
        return;
    sig_state(UNSET_SIGS, _SAVE);
    pid_t pid = fork();
    if (pid == 0)
    {
        child_singal_handler();
        if ((*root)->type == CMD && (*root)->cmd->input) // removed the check for (*root)->cmd if it exists
        {
            tmp = (*root)->cmd->input;
            tmp_og = (*root)->cmd->og_tokens->og_input;
            while(tmp)
            {
                if (tmp->mode == HEREDOC && tmp->file_name)
                        here_doc(exec , tmp_og, tmp, error_flag);
                tmp = tmp->next;
                tmp_og = tmp_og->next;
            }
        }
        exit (0);
    }
    waitpid(pid, &status, WUNTRACED);
    sig_state(SET_SIGS, _SAVE);
    if (WEXITSTATUS(status) == 120)
        ft_print_error(NULL, NULL, RESET_HEREDOC);
    _error_expand_heredoc_to_infiles(exec , &(*root)->l_token, error_flag);
    _error_expand_heredoc_to_infiles(exec , &(*root)->r_token, error_flag);
}

void    open_heredoc(t_exec *exec, int w_heredoc, char *og_delimiter, char *delimiter)
{
    // int status;
    pid_t pid = fork();
    if (pid == 0)
    {
        child_singal_handler();
        here_doc_helper(exec , w_heredoc, og_delimiter, delimiter); // to change the TRUE flag later
    }
    else
    {
        waitpid(pid, NULL, WUNTRACED);
        return;
    }
}

static void heredoc_to_fds(t_token **root)
{
    t_redir *tmp;
    if (!*root)
        return;
    if((*root)->type == CMD && (*root)->cmd->input)
    {
        tmp =  (*root)->cmd->input;
        while(tmp)
        {
            if (tmp->mode == HEREDOC)
                expand_heredoc(&tmp);
            tmp = tmp->next;
        }
    }
    heredoc_to_fds(&((*root)->l_token));
    heredoc_to_fds(&((*root)->r_token));
}

void    minishell_loop(char **env)
{
    char    *line;
    char    *og_line;
    t_exec  exec;
    t_token *head_tokens;

    exec.env = parse_env(env);
    exec.envp = env_to_envp(&exec);
    head_tokens = NULL;
    while(42)
    {
        exec.out = dup(STDIN_FILENO);
        exec.out = dup(STDOUT_FILENO);    
        exec.env = parse_env(env);
        exec.envp = env_to_envp(&exec);
        singal_handler(); 
        og_line = readline(GREEN"minishell$ " "\033[35m");
        line = og_line;
        if (!line)
            break;
        if (line)
		    add_history(line);
        special_trim(&line);
        head_tokens = lexer_manager(&line);
        display(head_tokens);
        evaluate_syntax(head_tokens);
        heredoc_to_fds(&head_tokens);
        if (ft_print_error(NULL, NULL, RETRIEVE) == TRUE)
            _error_expand_heredoc_to_infiles(&exec, &head_tokens, TRUE);
        ft_print_error(NULL, NULL, PRINT);
        if (ft_print_error(NULL, NULL, RETRIEVE) == FALSE)
            execute(head_tokens, &exec);
        ft_print_error(NULL, NULL, RESET);
        if (*line)
            printf("WTF LINE ISN'T EMPTY : line is %s\n", line);
        free(og_line);
        cleanup(head_tokens);
        head_tokens = NULL;
        // close(exec.in);
        // close(exec.out);
    }
}

int main(int ac, char **av, char **env)
{
    (void)av;
    if (ac == 1)
        minishell_loop(env);
}
