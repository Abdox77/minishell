// #include "minishell.h"
// void ambiguous()
// {
//     ft_write("amibiguous redirect", 2, 1);
//     exit(1);
// }

// static char *expand_arg_if_needed1(char *arg, char *og_arg, t_env *env_list)
// {
//     (void)og_arg;
//     char *expanded;
//     char **tmp;
//     char **tmp2;

//     if(ft_strchr(arg, ' ') || ft_strchr(arg, '\t'))
//         ambiguous();
//     if (ft_strchr(arg, '*') || ft_strchr(arg, '?'))
//     {
//             puts("HHHHHdfsdfHHH");

//         tmp = initialize_args_if_null(arg, NULL);
//         tmp2 = expand_wildcards(tmp);
//         if(tmp2[1])
//             ambiguous();
//         free(tmp);
//         // free(expanded);
//         expanded = ft_strdup(tmp2[0]);
//         free(tmp2);
//         return (expanded);
//     }
//     if (strchr(arg, '$'))
//     {
//         expanded = expand_string(arg, env_list); // Need to do this with og_args
//             puts("HHHHHHfdsfsH");

//         if(ft_strchr(expanded, ' ') || ft_strchr(expanded, '\t'))
//             ambiguous();
//         if (ft_strchr(expanded, '*') || ft_strchr(expanded, '?'))
//         {
//             puts("HHHHHHHH");
//             tmp = initialize_args_if_null(expanded, NULL);
//             tmp2 = expand_wildcards(tmp);
//             if(tmp[1])
//                 ambiguous();
//             free(tmp);
//             free(expanded);
//             expanded = ft_strdup(tmp2[0]);
//             free(tmp2);
//         }
//         return (expanded);
//     }
//     return strdup(arg);
// }

// void handle_input_redirections(t_redir *input, t_redir *og_input, t_env *env, t_exec *exec)
// {
//     int fd;
//     char *expanded_filename;
//     char *line;
//     char *expanded_line;

//     while (input)
//     {
//         if (input->mode == INFILE)
//         {
//             expanded_filename = expand_arg_if_needed1(input->file_name, og_input->file_name, env);
//             puts(expanded_filename);
//             fd = open(expanded_filename, O_RDONLY);
//             if (fd < 0)
//             {
//                 perror("Failed to open input file");
//                 free(expanded_filename);
//                 exit(1);
//             }
//             if (dup2(fd, STDIN_FILENO) < 0)
//             {
//                 perror("dup2 failed for input redirection");
//                 free(expanded_filename);
//                 exit(1);
//             }
//             close(fd);
//         }
//         else if (input->mode == HEREDOC)
//         {
//             expanded_filename = ft_strdup(input->file_name);
//             if (og_input->to_be_expanded == TRUE)
//             {
//                 unlink("FNAME");
//                 input->here_doc_fd[_HEREDOC_EXPAND_FD] = open("FNAME",O_CREAT | O_RDWR | O_TRUNC, 0644);
//                 unlink(FNAME);
//                 // printf("%d", input->here_doc_fd[R_HEREDOC]);   
//                 int expanded_fd = input->here_doc_fd[_HEREDOC_EXPAND_FD];
//                 line = get_next_line(input->here_doc_fd[R_HEREDOC]);
//                 while (line)
//                 {
//                     expanded_line = expand_in_heredoc(exec,line);
//                     // printf("here is the line %s \n", expanded_line);
//                     write(expanded_fd, expanded_line, ft_strlen(expanded_line));
//                     write(expanded_fd, "\n", 1);
//                     free(line);
//                     free(expanded_line);
//                     line = get_next_line(input->here_doc_fd[R_HEREDOC]);
//                 }
//                 int duppy = open("FNAME", O_RDWR, 0644);
//                 unlink("FNAME");
//                 // printf("%d", expanded_fd);
//                 if (dup2(duppy, STDIN_FILENO) < 0)
//                 {
//                     perror("dup2 failed for input redirection");
//                     exit(1);
//                 }
//                 close(expanded_fd);
//                 // close(fdd);
//                 close(input->here_doc_fd[R_HEREDOC]);

//             }
//             else
//             {
//                 // open_heredoc(exec, input->here_doc_fd[1], og_input->file_name, input->file_name);
//                 if (dup2(input->here_doc_fd[0], STDIN_FILENO) < 0)
//                 {
//                     perror("dup2 failed for input redirection");
//                     exit(1);
//                 }
//                 close(input->here_doc_fd[0]);
//             }
//         }
//         free(expanded_filename);
//         input = input->next;
//         og_input = og_input->next;
//     }
// }

// void handle_output_redirections(t_redir *output, t_redir *og_output, t_env *env)
// {
//     int fd;
//     char *expanded_filename;

//     while (output)
//     {
//         if (check_to_expand(output->file_name, env))
//         {
//             expanded_filename = expand_arg_if_needed1(output->file_name, output->file_name, env);
//             if (output->mode == OUTFILE || output->mode == TRUNC)
//                 fd = open(expanded_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             else if (output->mode == APPEND)
//                 fd = open(expanded_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             if (fd < 0)
//             {
//                 perror("Failed to open output file");
//                 free(expanded_filename);
//                 exit(EXIT_FAILURE);
//             }
//             if (dup2(fd, STDOUT_FILENO) < 0)
//             {
//                 perror("dup2 failed for output redirection");
//                 free(expanded_filename);
//                 exit(EXIT_FAILURE);
//             }
//         }
//         else
//         {
//             ft_write("ambigous redirect", 2, 1);
//             exit(1);
//         }
//         close(fd);
//         free(expanded_filename);
//         output = output->next;
//         og_output = og_output->next;
//     }
// }

// void handle_redirections(t_cmd *cmd, t_env *env, t_exec *exec)
// {
//     handle_output_redirections(cmd->output, cmd->og_tokens->og_output, env);
//     handle_input_redirections(cmd->input, cmd->og_tokens->og_input, env, exec);
// }

#include "minishell.h"
#include <dirent.h>

void ambiguous(void)
{
    ft_write("ambiguous redirect", 2, 1);
    exit(1);
}

static char *handle_wildcards(char *arg)
{
    char **tmp;
    char **tmp2;
    char *expanded;

    tmp = initialize_args_if_null(arg, NULL);
    tmp2 = expand_wildcards(tmp);
    if (tmp2[1])
        ambiguous();
    free(tmp);
    expanded = ft_strdup(tmp2[0]);
    free(tmp2);
    return (expanded);
}

static char *expand_arg_if_needed1(char *arg, char *og_arg, t_env *env_list)
{
    char *expanded;

    (void)og_arg;
    if (ft_strchr(arg, ' ') || ft_strchr(arg, '\t'))
        ambiguous();
    if (ft_strchr(arg, '*') || ft_strchr(arg, '?'))
        return handle_wildcards(arg);
    if (strchr(arg, '$'))
    {
        expanded = expand_string(arg, env_list);
        if (ft_strchr(expanded, ' ') || ft_strchr(expanded, '\t'))
            ambiguous();
        if (ft_strchr(expanded, '*') || ft_strchr(expanded, '?'))
        {
            // free(expanded);
            return handle_wildcards(expanded);
        }
        return (expanded);
    }
    return (strdup(arg));
}

void handle_infile(t_redir *input, t_redir *og_input, t_env *env)
{
    int fd;
    char *expanded_filename;

    expanded_filename = expand_arg_if_needed1(input->file_name, og_input->file_name, env);
    fd = open(expanded_filename, O_RDONLY);
    if (fd < 0)
    {
        perror(expanded_filename);
        free(expanded_filename);
        exit(1);
    }
    if (dup2(fd, STDIN_FILENO) < 0)
    {
        perror("dup2 failed for input redirection");
        free(expanded_filename);
        exit(1);
    }
    close(fd);
    free(expanded_filename);
}

void handle_heredoc_expansion(t_redir *input, t_exec *exec)
{
    char *line;
    char *expanded_line;
    int expanded_fd;

    unlink("FNAME");
    input->here_doc_fd[_HEREDOC_EXPAND_FD] = open("FNAME", O_CREAT | O_RDWR | O_TRUNC, 0644);
    unlink(FNAME);
    expanded_fd = input->here_doc_fd[_HEREDOC_EXPAND_FD];
    line = get_next_line(input->here_doc_fd[R_HEREDOC]);
    while (line)
    {
        expanded_line = expand_in_heredoc(exec, line);
        write(expanded_fd, expanded_line, ft_strlen(expanded_line));
        write(expanded_fd, "\n", 1);
        free(line);
        free(expanded_line);
        line = get_next_line(input->here_doc_fd[R_HEREDOC]);
    }
}

void handle_heredoc(t_redir *input, t_redir *og_input, t_exec *exec)
{
    int duppy;

    if (og_input->to_be_expanded == TRUE)
    {
        handle_heredoc_expansion(input, exec);
        duppy = open("FNAME", O_RDWR, 0644);
        unlink("FNAME");
        if (dup2(duppy, STDIN_FILENO) < 0)
        {
            perror("dup2 failed for input redirection");
            exit(1);
        }
        close(input->here_doc_fd[_HEREDOC_EXPAND_FD]);
        close(input->here_doc_fd[R_HEREDOC]);
    }
    else
    {
        if (dup2(input->here_doc_fd[0], STDIN_FILENO) < 0)
        {
            perror("dup2 failed for input redirection");
            exit(1);
        }
        close(input->here_doc_fd[0]);
    }
}

void handle_input_redirections(t_redir *input, t_redir *og_input, t_env *env, t_exec *exec)
{
    while (input)
    {
        if (input->mode == INFILE)
            handle_infile(input, og_input, env);
        else if (input->mode == HEREDOC)
            handle_heredoc(input, og_input, exec);
        input = input->next;
        og_input = og_input->next;
    }
}

void open_output_file(t_redir *output, char *expanded_filename)
{
    int fd;

    fd = -1;
    if (output->mode == OUTFILE || output->mode == TRUNC)
        fd = open(expanded_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (output->mode == APPEND)
        fd = open(expanded_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror(expanded_filename);
        free(expanded_filename);
        exit(EXIT_FAILURE);
    }
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 failed for output redirection");
        free(expanded_filename);
        exit(EXIT_FAILURE);
    }
    close(fd);
}

void handle_output_redirections(t_redir *output, t_redir *og_output, t_env *env)
{
    char *expanded_filename;

    while (output)
    {
        if (check_to_expand(output->file_name, env))
        {
            expanded_filename = expand_arg_if_needed1(output->file_name, output->file_name, env);
            open_output_file(output, expanded_filename);
            free(expanded_filename);
        }
        else
        {
            ft_write("ambiguous redirect", 2, 1);
            exit(1);
        }
        output = output->next;
        og_output = og_output->next;
    }
}

void handle_redirections(t_cmd *cmd, t_env *env, t_exec *exec)
{
    handle_output_redirections(cmd->output, cmd->og_tokens->og_output, env);
    handle_input_redirections(cmd->input, cmd->og_tokens->og_input, env, exec);
}
