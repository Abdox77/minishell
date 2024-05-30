#include "minishell.h"

// void handle_input_redirection(t_redir *input)
// {
//     int fd = -1;

//     while (input)
//     {
//         if (input->mode == INFILE)
//         {
//             fd = open(input->file_name, O_RDONLY);
//             if (fd < 0)
//             {
//                 perror("Failed to open input file");
//                 // exit(EXIT_FAILURE);
//             }
//         }
//         input = input->next;
//     }
//     if (fd >= 0)
//     {
//         dup2(fd, STDIN_FILENO);
//         close(fd);
//     }
// }

// void handle_output_redirection(t_redir *output)
// {
//     int fd = -1;
//     while (output)
//     {
//         if (output->mode == OUTFILE || output->mode == TRUNC)
//             fd = open(output->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         else if (output->mode == APPEND)
//             fd = open(output->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
//         if (fd < 0)
//         {
//             perror("Failed to open output file");
//             // exit(EXIT_FAILURE);
//         }
//         output = output->next;
//     }
//     if (fd >= 0)
//     {
//         dup2(fd, STDOUT_FILENO);
//         close(fd);
//     }
// }

// void handle_redirections(t_cmd *cmd)
// {
//     int old_stdin = dup(STDIN_FILENO);
//     int old_stdout = dup(STDOUT_FILENO);
//     handle_input_redirection(cmd->input);
//     handle_output_redirection(cmd->output);
//     dup2(STDOUT_FILENO, old_stdout);
//     dup2(STDIN_FILENO, old_stdin);
// }

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char *find_env_value(const char *var_name, t_env *env_list) {
    while (env_list != NULL) {
        if (ft_strcmp(var_name, env_list->key) == 0)
            return env_list->value;
        env_list = env_list->next;
    }
    return NULL;
}

static size_t get_expanded_length(const char *str, t_env *env_list) {
    size_t length = 0;

    while (*str) {
        if (*str == '$') {
            str++;
            const char *var_start = str;
            while (*str && (ft_isalnum(*str) || *str == '_'))
                str++;
            size_t var_len = str - var_start;
            char *var_name = ft_strndup(var_start, var_len);
            char *var_value = find_env_value(var_name, env_list);
            free(var_name);
            if (var_value) {
                length += ft_strlen(var_value);
            }
        } else if (*str == '"') {
            str++;
            while (*str && *str != '"') {
                if (*str == '$') {
                    str++;
                    const char *var_start = str;
                    while (*str && (ft_isalnum(*str) || *str == '_'))
                        str++;
                    size_t var_len = str - var_start;
                    char *var_name = ft_strndup(var_start, var_len);
                    char *var_value = find_env_value(var_name, env_list);
                    free(var_name);
                    if (var_value) {
                        length += ft_strlen(var_value);
                    }
                } else {
                    length++;
                    str++;
                }
            }
            if (*str == '"')
                str++;
        } else {
            length++;
            str++;
        }
    }

    return length;
}

static char *expand_string1(const char *str, t_env *env_list) {
    size_t result_size = get_expanded_length(str, env_list) + 1;
    char *result = (char *)malloc(result_size);
    if (!result) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    result[0] = '\0';
    char *result_ptr = result;

    while (*str) {
        if (*str == '$') {
            str++;
            const char *var_start = str;
            while (*str && (ft_isalnum(*str) || *str == '_'))
                str++;
            size_t var_len = str - var_start;
            char *var_name = ft_strndup(var_start, var_len);
            char *var_value = find_env_value(var_name, env_list);
            free(var_name);
            if (var_value) {
                strcpy(result_ptr, var_value);
                result_ptr += strlen(var_value);
            }
        } else if (*str == '"') {
            str++;
            while (*str && *str != '"') {
                if (*str == '$') {
                    str++;
                    const char *var_start = str;
                    while (*str && (ft_isalnum(*str) || *str == '_'))
                        str++;
                    size_t var_len = str - var_start;
                    char *var_name = ft_strndup(var_start, var_len);
                    char *var_value = find_env_value(var_name, env_list);
                    free(var_name);
                    if (var_value) {
                        strcpy(result_ptr, var_value);
                        result_ptr += ft_strlen(var_value);
                    }
                } else {
                    *result_ptr++ = *str++;
                }
            }
            if (*str == '"')
                str++;
        } else {
            *result_ptr++ = *str++;
        }
    }
    *result_ptr = '\0';
    return result;
}

static char *expand_arg_if_needed1(char *arg, char *og_arg, t_env *env_list) {
    if (strchr(arg, '$')) {
        return expand_string1(og_arg, env_list); // Need to do this with og_args
    }
    return strdup(arg);
}





// void handle_input_redirections(t_redir *input, t_env *env)
// {
//     int fd;

//     while (input)
//     {
//         if (input->mode == INFILE)
//         {
//             fd = open(input->file_name, O_RDONLY);
//             if (fd < 0)
//             {
//                 perror("Failed to open input file");
//                 exit(EXIT_FAILURE);
//             }
//             if (dup2(fd, STDIN_FILENO) < 0)
//             {
//                 perror("dup2 failed for input redirection");
//                 exit(EXIT_FAILURE);
//             }
//             close(fd);
//         }
//         input = input->next;
//     }
// }

// void handle_output_redirections(t_redir *output, t_env *env)
// {
//     int fd;

//     while (output)
//     {
//         if (output->mode == OUTFILE || output->mode == TRUNC)
//         {
//             fd = open(output->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (fd < 0)
//             {
//                 perror("Failed to open output file");
//                 exit(EXIT_FAILURE);
//             }
//             if (dup2(fd, STDOUT_FILENO) < 0)
//             {
//                 perror("dup2 failed for output redirection");
//                 exit(EXIT_FAILURE);
//             }
//             close(fd);
//         }
//         else if (output->mode == APPEND)
//         {
//             fd = open(output->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             if (fd < 0)
//             {
//                 perror("Failed to open output file");
//                 exit(EXIT_FAILURE);
//             }
//             if (dup2(fd, STDOUT_FILENO) < 0)
//             {
//                 perror("dup2 failed for append redirection");
//                 exit(EXIT_FAILURE);
//             }
//             close(fd);
//         }
//         output = output->next;
//     }
// }

// void handle_redirections(t_cmd *cmd, t_env *env)
// {
//     handle_input_redirections(cmd->input, env);
//     handle_output_redirections(cmd->output, env);
// }





void handle_input_redirections(t_redir *input, t_redir *og_input, t_env *env, t_exec *exec)
{
    int fd;
    char *expanded_filename;
    char *line;
    char *expanded_line;

    while (input)
    {
        if (input->mode == INFILE)
        {
            expanded_filename = expand_arg_if_needed1(input->file_name, og_input->file_name, env);
            fd = open(expanded_filename, O_RDONLY);
            if (fd < 0)
            {
                perror("Failed to open input file");
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
        }
        // else if (input->mode == HEREDOC)
        // {
        //     (void)exec;
        //     expanded_filename = ft_strdup(input->file_name);
        //     if(input->to_be_expanded == TRUE)
        //     {
        //         line = get_next_line(input->here_doc_fd[R_HEREDOC]);
        //         while(line)
        //         {
        //         }
        //     }
        //     if (dup2(input->here_doc_fd[R_HEREDOC], STDIN_FILENO) < 0)
        //     {
        //         perror("dup2 failed for input redirection");
        //         exit(1);
        //     }
        //     close(input->here_doc_fd[0]);

        // }
        else if (input->mode == HEREDOC)
        {
            // int fdd = open("heredoc", O_RDWR | O_CREAT | O_TRUNC, 0644);
            // dup2(input->here_doc_fd[R_HEREDOC], fdd);
            // unlink("heredoc");
            expanded_filename = ft_strdup(input->file_name);
            if (input->to_be_expanded == FALSE)
            {
                unlink("FNAME");
                input->here_doc_fd[_HEREDOC_EXPAND_FD] = open("FNAME",O_CREAT | O_RDWR | O_TRUNC, 0644);
                unlink(FNAME);
                // printf("%d", input->here_doc_fd[R_HEREDOC]);   
                int expanded_fd = input->here_doc_fd[_HEREDOC_EXPAND_FD];
                line = get_next_line(input->here_doc_fd[R_HEREDOC]);
                while (line)
                {
                    expanded_line = expand_in_heredoc(exec,line);
                    // printf("here is the line %s \n", expanded_line);
                    write(expanded_fd, expanded_line, ft_strlen(expanded_line));
                    write(expanded_fd, "\n", 1);
                    free(line);
                    free(expanded_line);
                    line = get_next_line(input->here_doc_fd[R_HEREDOC]);
                }
                int duppy = open("FNAME", O_RDWR, 0644);
                unlink("FNAME");
                // printf("%d", expanded_fd);
                if (dup2(duppy, STDIN_FILENO) < 0)
                {
                    perror("dup2 failed for input redirection");
                    exit(1);
                }
                close(expanded_fd);
                // close(fdd);
                close(input->here_doc_fd[R_HEREDOC]);

            }
            else
            {
                // open_heredoc(exec, input->here_doc_fd[1], og_input->file_name, input->file_name);
                if (dup2(input->here_doc_fd[0], STDIN_FILENO) < 0)
                {
                    perror("dup2 failed for input redirection");
                    exit(1);
                }
                close(input->here_doc_fd[0]);
            }
        }
        free(expanded_filename);
        input = input->next;
        og_input = og_input->next;
    }
}

void handle_output_redirections(t_redir *output, t_redir *og_output, t_env *env)
{
    int fd;
    char *expanded_filename;

    while (output)
    {
        if (check_to_expand(output->file_name, env))
        {
            expanded_filename = expand_arg_if_needed1(output->file_name, output->file_name, env);
            if(ft_strchr(expanded_filename, ' '))
            {
                ft_write("ambigous redirect", 2, 1);
                exit(1);
            }
            if (output->mode == OUTFILE || output->mode == TRUNC)
                fd = open(expanded_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else if (output->mode == APPEND)
                fd = open(expanded_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror("Failed to open output file");
                free(expanded_filename);
                exit(EXIT_FAILURE);
            }
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("dup2 failed for output redirection");
                free(expanded_filename);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            ft_write("ambigous redirect", 2, 1);
            exit(1);
        }
        close(fd);
        free(expanded_filename);
        output = output->next;
        og_output = og_output->next;
    }
}

void handle_redirections(t_cmd *cmd, t_env *env, t_exec *exec)
{
    int in;
    int out;
    in = dup(STDIN_FILENO);
    out = dup(STDOUT_FILENO);
    handle_output_redirections(cmd->output, cmd->og_tokens->og_output, env);
    handle_input_redirections(cmd->input, cmd->og_tokens->og_input, env, exec);
    dup2(STDIN_FILENO, in);
    dup2(STDOUT_FILENO, out);
}
