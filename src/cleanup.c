#include "minishell.h"

void free_strs(char **strs)
{
    int i;

    if (!strs)
        return;
    i = -1;
    while(strs[++i])
        free(strs[i]);
    free(strs);        
}

void free_redirections(t_redir *redir)
{
    t_redir *tmp;

    if (!redir)
        return;
    tmp = redir;
    while(tmp)
    {
        tmp = redir->next;
        free(redir->file_name);
        free(redir);
        redir = tmp;
    }
}

void free_expands(t_expands *expands)
{
    if (!expands)
        return;
    free(expands->og_cmd);
    free_strs(expands->og_args);
    free_redirections(expands->og_input);
    free_redirections(expands->og_output);
    free(expands);
}

void free_cmd(t_token *root)
{
    if (!root)
        return;
    free(root->cmd->cmd);
    free(root->cmd->cmd_to_be_expanded);
    free_strs(root->cmd->args);
    free_expands(root->cmd->og_tokens);
    free_redirections(root->cmd->input);
    free_redirections(root->cmd->output);
    free(root->cmd);
    free(root);
    root = NULL;
}

void free_token(t_token *root)
{
    if (!root)
        return;
    free_redirections(root->output);
    free_redirections(root->og_output);
    free_redirections(root->input);
    free_redirections(root->og_input);
}

void cleanup(t_token *root)
{
    if (!root)
        return;
    if (root && !root->l_token && !root->r_token)
    {
        if (root->type == CMD)
            free_cmd(root);
        else
            free_token(root);    
    }
    else
    {
        cleanup(root->l_token);
        cleanup(root->r_token);
        if(root->type == CMD)
            {
                printf("This is impossible yet it happened\n");
                free_cmd(root);}
        else
            free_token(root);
    }
}


void    ft_error(char *error_message, int exit_code)
{
    ft_putstr_fd(error_message, STDERR_FILENO);
    exit(exit_code);
}
