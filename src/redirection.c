#include "minishell.h"

void handle_input_redirection(t_redir *input)
{
    int fd = -1;

    while (input)
    {
        if (input->mode == INFILE)
        {
            fd = open(input->file_name, O_RDONLY);
            if (fd < 0)
            {
                perror("Failed to open input file");
                exit(EXIT_FAILURE);
            }
        }
        input = input->next;
    }
    if (fd >= 0)
    {
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
}

void handle_output_redirection(t_redir *output)
{
    int fd = -1;
    while (output)
    {
        if (output->mode == OUTFILE || output->mode == TRUNC)
            fd = open(output->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (output->mode == APPEND)
            fd = open(output->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0)
        {
            perror("Failed to open output file");
            exit(EXIT_FAILURE);
        }
        output = output->next;
    }
    if (fd >= 0)
    {
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void handle_redirections(t_cmd *cmd)
{
    handle_input_redirection(cmd->input);
    handle_output_redirection(cmd->output);
}
