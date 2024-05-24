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





void handle_input_redirections(t_redir *input, t_env *env)
{
    int fd;
    char *expanded_filename;

    while (input)
    {
        expanded_filename = expand_arg_if_needed1(input->file_name, input->file_name, env);
        if (input->mode == INFILE)
        {
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
        free(expanded_filename);
        input = input->next;
    }
}

void handle_output_redirections(t_redir *output, t_env *env)
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
    }
}

void handle_redirections(t_cmd *cmd, t_env *env)
{
    handle_input_redirections(cmd->input, env);
    handle_output_redirections(cmd->output, env);
}
