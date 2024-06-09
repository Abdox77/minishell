
#include "minishell.h"

static char *find_env_value(const char *var_name, t_env *env_list) {
    while (env_list != NULL) {
        if (strcmp(var_name, env_list->key) == 0)
            return env_list->value;
        env_list = env_list->next;
    }
    return NULL;
}

static size_t get_expanded_length(const char *str, t_env *env_list)
{
    size_t length = 0;
    while (*str)
    {
        if (*str == '$')
        {
            str++;
            const char *var_start = str;
            while (*str && (ft_isalnum(*str) || *str == '_'))
                str++;
            size_t var_len = str - var_start;
            char *var_name = ft_strndup(var_start, var_len);
            char *var_value = find_env_value(var_name, env_list);
            free(var_name);
            if (var_value)
                length += strlen(var_value);
        }
        else if (*str == '"')
        {
            str++;
            while (*str && *str != '"')
            {
                if (*str == '$')
                {
                    str++;
                    const char *var_start = str;
                    while (*str && (isalnum(*str) || *str == '_'))
                        str++;
                    size_t var_len = str - var_start;
                    char *var_name = strndup(var_start, var_len);
                    char *var_value = find_env_value(var_name, env_list);
                    free(var_name);
                    if (var_value)
                        length += strlen(var_value);
                }
                else
                {
                    length++;
                    str++;
                }
            }
            if (*str == '"')
                str++;
        }
        else
        {
            length++;
            str++;
        }
    }
    return length;
}
static char *expand_string1(const char *str, t_env *env_list) {
    size_t result_size = get_expanded_length(str, env_list) + 1;
    char *result = (char *)malloc(result_size);
    if (!result)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    result[0] = '\0';
    char *result_ptr = result;

    while (*str)
    {
        if (*str == '$')
        {
            str++;
            const char *var_start = str;
            while (*str && (isalnum(*str) || *str == '_'))
                str++;
            size_t var_len = str - var_start;
            char *var_name = strndup(var_start, var_len);
            char *var_value = find_env_value(var_name, env_list);
            free(var_name);
            if (var_value)
            {   
                strcpy(result_ptr, var_value);
                result_ptr += strlen(var_value);
            }
        }
        else if (*str == '"')
        {
            str++;
            while (*str && *str != '"')
            {
                if (*str == '$')
                {
                    str++;
                    const char *var_start = str;
                    while (*str && (isalnum(*str) || *str == '_'))
                        str++;
                    size_t var_len = str - var_start;
                    char *var_name = strndup(var_start, var_len);
                    char *var_value = find_env_value(var_name, env_list);
                    free(var_name);
                    if (var_value)
                    {   
                        strcpy(result_ptr, var_value);
                        result_ptr += strlen(var_value);
                    }
                }
                else
                    *result_ptr++ = *str++;
            }
            if (*str == '"')
                str++;
        }
        else
            *result_ptr++ = *str++;
    }
    *result_ptr = '\0';
    return result;
}

static int is_expandable_char(char c)
{
    return (isalnum(c) || c == '_' || c== '$');
}

static int is_quoted(const char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == '"' || str[i] == '\'')
            return 1;
        i++;
    }
    return 0;
}

 int check_to_expand(char *cmd, t_env *env_list)
{
    (void)env_list;
    int i = 0;
    if (!cmd || cmd[i] != '$')
        return (printf("because of 0; \n"),1);
    
    if (is_quoted(cmd))
        return (printf("ghsdldsbfldsbfljdsbflsdjbfs"),1);

    i++; // Skip the initial $

    if (!is_expandable_char(cmd[i]))
        return (printf("because of 1; \n"),1);
    
    while (cmd[i])
    {
        if (!is_expandable_char(cmd[i]))
        return (printf("because of 2; \n"),1);
        cmd++;
    }
    if(expand_string1(cmd, env_list)){

            printf("ghsdldsbfldsbfljdsbflsdjbfs");
           return (printf("because of 3; \n"),1);}
    return 0;
}

static char **initialize_args_if_null(char *cmd, char **args)
{
    if (args == NULL)
    {
        args = malloc(2 * sizeof(char*));
        if (args == NULL)
        {
            perror("Failed to allocate memory for args");
            exit(EXIT_FAILURE);
        }
        args[0] = ft_strdup(cmd);
        args[1] = NULL;
    }
    return args;
}

// static void handle_signals(void)
// {
//     signal(SIGINT, SIG_DFL);
// }

// // void execute_command(t_token *token, t_exec *exec)
// // {
// //     exec->envp = env_to_envp(exec);
// //     char *cmd = token->cmd->cmd;
// //     char **args = token->cmd->args;
// //     int flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);
// //     args = initialize_args_if_null(cmd, args);
// //     args = process_args(args, token->cmd->og_tokens->og_args, token->cmd->og_tokens->og_cmd, cmd, exec->env);
// //     args = expand_wildcards(args);
// //     if(args)
// //         cmd = args[0];
// //     else
// //         cmd = "";
// //     if (token->cmd->cmd && check_builtins(token, exec, args))
// //         return;
// //     pid_t pid = fork();
// //     if (pid < 0)
// //         perror("Fork failed");
// //     else if (pid == 0)
// //     {
// //         handle_signals();
// //         handle_redirections(token->cmd, exec->env);
// //     printf("flag is ________________--%d================== \n ", flag); /////////////////////////////////now i need you to create an another thing please which is i.e     args[0]=ls args[1]=*.c args[2]=-la (the directory has test.c and 1.c); you give a 2d array args[0]=ls args[1]=test.c ags[2]=1.c args[3]=-la i hope you understand

// //         if(!flag)
// //         {
// //             stat(0, 1);
// //             exit(0);
// //             return;
// //         }
// //             printf("args are========%s \n", args[0]);
// //             // else
// //             // {
// //                 char *cmd_path = get_cmd(cmd, exec->envp);
// //                 printf("===============executing======= %s \n \n", cmd_path);
// //                 execve(cmd_path, args, exec->envp);
// //                 ft_write(cmd,2,0);
// //                 ft_write(": command not found", 2, 1);
// //                 stat(127, 1);
// //                 exit(127);
// //             // }
// //     }
// //     else
// //     {
// //         int status;
// //         waitpid(pid, &status, 0);
// //         stat(WEXITSTATUS(status), 1);
// //     }
// // }

// void execute_command(t_token *token, t_exec *exec)
// {
//     exec->envp = env_to_envp(exec);
//     char *cmd = token->cmd->cmd;
//     char **args = token->cmd->args;
//     int flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);
//     args = initialize_args_if_null(cmd, args);
//     args = process_args(args, token->cmd->og_tokens->og_args, token->cmd->og_tokens->og_cmd, cmd, exec->env);
//     args = expand_wildcards(args);
//     if (*args)
//         cmd = args[0];
//     else
//         cmd = "\0";
//     if (token->cmd->cmd && check_builtins(token, exec, args))
//         return;
//     pid_t pid = fork();
//     if (pid < 0)
//         perror("Fork failed");
//     else if (pid == 0)
//     {
//         handle_redirections(token->cmd, exec->env);
//         handle_signals();
//         printf("flag is ________________--%d================== \n ", flag);

//         if (!flag)
//         {
//             stat(0, 1);
//             exit(0);
//             return;
//         }
//         if (cmd)
//         {
//             printf("args are========%s \n", args[0]);
//             char *cmd_path = get_cmd(cmd, exec->envp);
//             printf("===============executing======= %s \n \n", cmd_path);
//             execve(cmd_path, args, exec->envp);
//             perror("execve failed");
//             ft_write(cmd, 2, 0);
//             ft_write(": command not found", 2, 1);
//             stat(127, 1);
//             free(cmd_path);
//             free_2d(args);
//             exit(127);
//         }
//     }
//     else
//     {
//         int status;
//         waitpid(pid, &status, 0);
//         stat(WEXITSTATUS(status), 1);
//         free_2d(args);
//     }
// }

// static void execute_left(t_token *node, int *fd, t_exec *exec)
// {
//     dup2(fd[1], STDOUT_FILENO);
//     close(fd[0]); // close the read end of the pipe
//     close(fd[1]); // close the write end of the pipe in child process
//     execute(node, exec);
//     exit(0);
// }

// static void execute_right(t_token *node, int *fd, t_exec *exec)
// {
//     dup2(fd[0], STDIN_FILENO);
//     close(fd[0]); // close the read end of the pipe in child process
//     close(fd[1]); // close the write end of the pipe
//     execute(node, exec);
//     exit(0);
// }

// static void execute_pipe(t_token *node, t_exec *exec)
// {
//     int fd[2];
//     pid_t pid1, pid2;
//     int status1, status2;

//     if (pipe(fd) == -1)
//     {
//         perror("Pipe failed");
//         exit(EXIT_FAILURE);
//     }

//     pid1 = fork();
//     if (pid1 == -1)
//     {
//         perror("Fork failed for left command");
//         exit(EXIT_FAILURE);
//     } else if (pid1 == 0)
//     {
//         close(fd[0]); // close read end of the pipe
//         execute_left(node->l_token, fd, exec);
//     }

//     pid2 = fork();
//     if (pid2 == -1) {
//         perror("Fork failed for right command");
//         exit(EXIT_FAILURE);
//     } else if (pid2 == 0) {
//         close(fd[1]);
//         execute_right(node->r_token, fd, exec);
//     }
//     close(fd[0]);
//     close(fd[1]);
//     waitpid(pid1, &status1, 0);
//     waitpid(pid2, &status2, 0);
//     stat(WEXITSTATUS(status2), 1);
// }

// // void execute_subtree(t_token *token, t_exec *exec)
// // {
// //     int saved_stdin = dup(STDIN_FILENO);
// //     int saved_stdout = dup(STDOUT_FILENO);

// //     if (saved_stdin == -1 || saved_stdout == -1)
// //     {
// //         perror("Failed to duplicate file descriptor");
// //         exit(EXIT_FAILURE);
// //     }

// //     if (token->input)
// //     {
// //         handle_input_redirections(token->input, exec->env);
// //     }

// //     if (token->output)
// //     {
// //         handle_output_redirections(token->output, exec->env);
// //     }

// //     execute(token, exec);

// //     if (dup2(saved_stdin, STDIN_FILENO) == -1)
// //     {
// //         perror("Failed to restore stdin");
// //         exit(EXIT_FAILURE);
// //     }
// //     if (dup2(saved_stdout, STDOUT_FILENO) == -1)
// //     {
// //         perror("Failed to restore stdout");
// //         exit(EXIT_FAILURE);
// //     }

// //     close(saved_stdin);
// //     close(saved_stdout);
// // }
// // void execute_subtree(t_token *root, t_exec *exec) {
// //     if (root == NULL) {
// //         return;
// //     }

// //     // Save original stdin and stdout file descriptors
// //     int original_stdin = dup(STDIN_FILENO);
// //     int original_stdout = dup(STDOUT_FILENO);

// //     // Redirect stdin if necessary
// //     if (root->input) {
// //         handle_input_redirections(root->input, exec->env);
// //     }

// //     // Redirect stdout if necessary
// //     if (root->output) {
// //         handle_output_redirections(root->output, exec->env);
// //     }

// //     // Execute the command or further traverse the tree
// //     if (root->type == CMD) {
// //         if (fork() == 0)
// //         {
// //             execute_command(root, exec);
// //             _exit(1); // Only exit the child process
// //         }
// //         else
// //         {
// //             wait(NULL); // Wait for the child process to finish
// //         }
// //     }
// //     else
// //     {
// //         execute_subtree(root->l_token, exec);
// //         execute_subtree(root->r_token, exec);
// //     }

// //     dup2(original_stdin, STDIN_FILENO);
// //     dup2(original_stdout, STDOUT_FILENO);

// //     close(original_stdin);
// //     close(original_stdout);
// // }

// // void execute(t_token *token, t_exec *exec)
// // {
// //     if (token == NULL)
// //         return;

// //     if (token->input || token->output)
// //     {
// //         execute_subtree(token, exec);
// //         return;
// //     }

// //     if (token->type == CMD && token->cmd->cmd) // Execute a simple command
// //     {
// //         execute_command(token, exec);
// //     }
// //     else if (token->type == PIPE)
// //     {
// //         execute_pipe(token, exec);
// //     }
// //     else if (token->type == AND)
// //     {
// //         execute(token->l_token, exec);
// //         if (stat(0, 0) == 0)
// //             execute(token->r_token, exec);
// //     }
// //     else if (token->type == OR)
// //     {
// //         execute(token->l_token, exec);
// //         if (stat(0, 0) != 0)
// //             execute(token->r_token, exec);
// //     }
// // }


// // void execute_and(t_token *node, t_exec *exec)
// // {
// //     execute(node->l_token, exec);
// //     if (stat(0, 0) == 0)
// //         execute(node->r_token, exec);
// //     // return status;
// // }

// // void execute_or(t_token *node, t_exec *exec)
// // {
// //     execute(node->l_token, exec);
// //     if (stat(0, 0) != 0)
// //         execute(node->r_token, exec);
// //     // return status;
// // }

// void execute_subtree(t_token *root, t_exec *exec)
// {
//     if (root == NULL) 
//         return;
//     int original_stdin = dup(STDIN_FILENO);
//     int original_stdout = dup(STDOUT_FILENO);
//     if (root->input)
//         handle_input_redirections(root->input, exec->env);
//     if (root->output)
//         handle_output_redirections(root->output, exec->env);
//     if (root->type == CMD) {
//         if (fork() == 0)
//         {
//             execute_command(root, exec);
//             _exit(1);
//         }
//         else
//             wait(NULL);
//     }
//     else if (root->type == PIPE)
//     {
//         execute_pipe(root, exec);
//     }
//     else if (root->type == AND)
//     {
//         execute_subtree(root->l_token, exec);
//         if (stat(0, 0) == 0)
//             execute_subtree(root->r_token, exec);
//     }
//     else if (root->type == OR)
//     {
//         execute_subtree(root->l_token, exec);
//         if (stat(0, 0) != 0)
//             execute_subtree(root->r_token, exec);
//     }
//     dup2(original_stdin, STDIN_FILENO);
//     dup2(original_stdout, STDOUT_FILENO);
//     close(original_stdin);
//     close(original_stdout);
// }

// void execute(t_token *token, t_exec *exec)
// {
//     if (token == NULL)
//         return;

//     if (token->input || token->output)
//     {
//         execute_subtree(token, exec);
//         return;
//     }

//     if (token->type == CMD)
//     {
//         execute_command(token, exec);
//     }
//     else if (token->type == PIPE)
//     {
//         execute_pipe(token, exec);
//     }
//     else if (token->type == AND)
//     {
//         execute_and(token, exec);
//     }
//     else if (token->type == OR)
//     {
//         execute_or(token, exec);
//     }
// }

// void execute_and(t_token *node, t_exec *exec)
// {
//     execute(node->l_token, exec);
//     if (stat(0, 0) == 0)
//         execute(node->r_token, exec);
// }

// void execute_or(t_token *node, t_exec *exec)
// {
//     execute(node->l_token, exec);
//     if (stat(0, 0) != 0)
//         execute(node->r_token, exec);
// }

#include "minishell.h"

// static char *find_env_value(const char *var_name, t_env *env_list)
// {
//     while (env_list)
//     {
//         if (strcmp(var_name, env_list->key) == 0)
//             return env_list->value;
//         env_list = env_list->next;
//     }
//     return NULL;
// }

// static size_t get_var_length(const char **str, t_env *env_list)
// {
//     const char *var_start = *str;
//     while (**str && (isalnum(**str) || **str == '_'))
//         (*str)++;
//     size_t var_len = *str - var_start;
//     char *var_name = ft_strndup(var_start, var_len);
//     char *var_value = find_env_value(var_name, env_list);
//     free(var_name);
//     return var_value ? strlen(var_value) : 0;
// }

// static size_t get_expanded_length(const char *str, t_env *env_list) {
//     size_t length = 0;
//     while (*str) {
//         if (*str == '$') {
//             str++;
//             length += get_var_length(&str, env_list);
//         } else if (*str == '"') {
//             str++;
//             while (*str && *str != '"') {
//                 if (*str == '$') {
//                     str++;
//                     length += get_var_length(&str, env_list);
//                 } else {
//                     length++;
//                     str++;
//                 }
//             }
//             if (*str == '"') str++;
//         } else {
//             length++;
//             str++;
//         }
//     }
//     return length;
// }

// static void append_expanded_var(char **result_ptr, const char **str, t_env *env_list) {
//     const char *var_start = *str;
//     while (**str && (isalnum(**str) || **str == '_'))
//         (*str)++;
//     size_t var_len = *str - var_start;
//     char *var_name = ft_strndup(var_start, var_len);
//     char *var_value = find_env_value(var_name, env_list);
//     free(var_name);
//     if (var_value) {
//         strcpy(*result_ptr, var_value);
//         *result_ptr += strlen(var_value);
//     }
// }

// static void handle_expansion(char **result_ptr, const char **str, t_env *env_list) {
//     if (**str == '$') {
//         (*str)++;
//         append_expanded_var(result_ptr, str, env_list);
//     } else if (**str == '"') {
//         (*str)++;
//         while (**str && **str != '"') {
//             if (**str == '$') {
//                 (*str)++;
//                 append_expanded_var(result_ptr, str, env_list);
//             } else {
//                 **result_ptr = **str;
//                 (*result_ptr)++;
//                 (*str)++;
//             }
//         }
//         if (**str == '"') (*str)++;
//     } else {
//         **result_ptr = **str;
//         (*result_ptr)++;
//         (*str)++;
//     }
// }

// static char *expand_string1(const char *str, t_env *env_list) {
//     size_t result_size = get_expanded_length(str, env_list) + 1;
//     char *result = malloc(result_size);
//     if (!result) {
//         perror("malloc failed");
//         exit(EXIT_FAILURE);
//     }
//     char *result_ptr = result;
//     while (*str)
//         handle_expansion(&result_ptr, &str, env_list);
//     *result_ptr = '\0';
//     return result;
// }

// static int is_expandable_char(char c) {
//     return isalnum(c) || c == '_' || c == '$';
// }

// static int is_quoted(const char *str) {
//     while (*str) {
//         if (*str == '"' || *str == '\'')
//             return 1;
//         str++;
//     }
//     return 0;
// }
// int check_to_expand(char *cmd, t_env *env_list)
// {
//     int result;
//     char *str;
//     result = 1;
//     if (!cmd || cmd[0] != '$')
//         return 1;
//     else if (is_quoted(cmd))
//         return 1;
//     else {
//         int i = 1;
//         while (cmd[i])
//         {
//             if (!is_expandable_char(cmd[i]))
//             {
//                 result = 1;
//                 break;
//             }
//             i++;
//         }
//         str = expand_string1(cmd, env_list);
//         if (str)
//             result = 1;
//         else
//             result = 0;
//     }
//     return (free(str), str = NULL, result);
// }

// static char **initialize_args_if_null(char *cmd, char **args) {
//     if (!args) {
//         args = malloc(2 * sizeof(char *));
//         if (!args) {
//             perror("Failed to allocate memory for args");
//             exit(EXIT_FAILURE);
//         }
//         args[0] = strdup(cmd);
//         args[1] = NULL;
//     }
//     return args;
// }

static void handle_signals(void) {
    signal(SIGINT, SIG_DFL);
}

char **expander(t_token *token, t_exec *exec, char *cmd)
{
    // char **args;
    char **processed_args;
    char **expanded_wildcards;

    exec->to_free = initialize_args_if_null(cmd, token->cmd->args);
    processed_args = process_args(exec->to_free, token->cmd->og_tokens->og_args, token->cmd->og_tokens->og_cmd, cmd, exec->env);
    // free_strs(args);
    expanded_wildcards = expand_wildcards(processed_args);
    free_strs(processed_args);
    return expanded_wildcards;
}

// void execute_command(t_token *token, t_exec *exec)
// {
//     exec->envp = env_to_envp(exec);
//     char *cmd = token->cmd->cmd;
//     // char **args = initialize_args_if_null(cmd, token->cmd->args);
//     // args = process_args(args, token->cmd->og_tokens->og_args, token->cmd->og_tokens->og_cmd, cmd, exec->env);
//     // args = expand_wildcards(args);
//     char **args = expander(token, exec, cmd);
//     int flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);
//     if (args[0])
//         cmd = args[0];
//     else
//         cmd = "\0";
//     if (check_builtins(cmd, exec, args))
//         return;
//     if (fork() == 0) {
//         handle_signals();
//         handle_redirections(token->cmd, exec->env, exec);
//         if (!flag) {
//             stat(0, 1);
//             exit(0);
//         }
//         char *cmd_path = get_cmd(cmd, exec->envp);
//         execve(cmd_path, args, exec->envp);
//         perror("execve failed");
//         free(cmd_path);
//         free_strs(args);
//         exit(127);
//     }
//     int status;
//     free_strs(args);
//     wait(&status);
//     stat(WEXITSTATUS(status), 1);
// }

// void execute_command(t_token *token, t_exec *exec) {
//     exec->envp = env_to_envp(exec);
//     char *cmd = token->cmd->cmd;
//     char **args = expander(token, exec, cmd);
//     int flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);

//     if (args[0])
//         cmd = args[0];
//     else
//         cmd = "\0";

//     if (check_builtins(cmd, exec, args)) {
//         free_strs(exec->envp);
//         free_strs(args);
//         return;
//     }

//     if (fork() == 0) {
//         handle_signals();
//         handle_redirections(token->cmd, exec->env, exec);

//         if (!flag) {
//             stat(0, 1);
//             exit(0);
//         }

//         char *cmd_path = get_cmd(cmd, exec->envp);
//         if (cmd_path)
//             execve(cmd_path, args, exec->envp);
//         perror("execve failed");
//         free(cmd_path);
//         free_strs(args);
//         free_strs(exec->envp);
//         exit(127);
//     }

//     int status;
//     wait(&status);
//     free_strs(args);
//     free_strs(exec->envp);
//     stat(WEXITSTATUS(status), 1);
// }

// void execute_command(t_token *token, t_exec *exec) {
//     exec->envp = env_to_envp(exec);
//     char *cmd = token->cmd->cmd;
//     char **args = expander(token, exec, cmd);
//     int flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);

//     if (args[0])
//         cmd = args[0];
//     else
//         cmd = "\0";

//     if (check_builtins(cmd, exec, args)) {
//         free_strs(exec->envp);
//         free_strs(args);
//         return;
//     }

//     if (fork() == 0) {
//         handle_signals();
//         handle_redirections(token->cmd, exec->env, exec);

//         if (!flag) {
//             stat(0, 1);
//             exit(0);
//         }

//         char *cmd_path = get_cmd(cmd, exec->envp);
//         execve(cmd_path, args, exec->envp);
//         perror("execve failed");
//         free(cmd_path);
//         free_strs(args);
//         free_strs(exec->envp);
//         exit(127);
//     }

//     int status;
//     wait(&status);
//     free_strs(args);
//     free_strs(exec->envp);
//     stat(WEXITSTATUS(status), 1);
// }

void execute_command(t_token *token, t_exec *exec) {
    exec->envp = env_to_envp(exec);
    int in = dup(STDIN_FILENO);
    int out = dup(STDOUT_FILENO);

    char *cmd = token->cmd->cmd;
    char **args = expander(token, exec, cmd);
    int flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);

    if (args[0])
        cmd = args[0];
    else
        cmd = "\0";

    if (check_builtins(cmd, token->cmd, exec, args)) {
        free_strs(exec->envp);
        exec->envp = NULL;
        free_strs(args);
        args = NULL;
        reset_fd(in, out);
        return;
    }

    // handle_here_doc(); // TODO
    if (fork() == 0) {
        handle_signals();
        handle_redirections(token->cmd, exec->env, exec);

        if (!flag) {
            stat(0, 1);
            exit(0);
        }

        char *cmd_path = get_cmd(cmd, exec->envp);
        execve(cmd_path, args, exec->envp);
        perror("execve failed");
        free(cmd_path);
        free_strs(args);
        args = NULL;
        free_strs(exec->envp);
        exec->envp = NULL;
        exit(127);
    }

    int status;
    wait(&status);
    free_strs(args);
    args = NULL;
    free_strs(exec->envp);
    exec->envp = NULL;
    stat(WEXITSTATUS(status), 1);
    reset_fd(in, out);
}

static void execute_left(t_token *node, int *fd, t_exec *exec) {
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    execute(node, exec);
    exit(0);
}

static void execute_right(t_token *node, int *fd, t_exec *exec) {
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);
    execute(node, exec);
    exit(0);
}

static void execute_pipe(t_token *node, t_exec *exec) {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }
    if (fork() == 0) {
        close(fd[0]);
        execute_left(node->l_token, fd, exec);
    }
    if (fork() == 0) {
        close(fd[1]);
        execute_right(node->r_token, fd, exec);
    }
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
}

void execute_subtree(t_token *root, t_exec *exec) {
    if (!root)
        return;
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    if (root->input)
        handle_input_redirections(root->input, root->og_input, exec->env, exec);
    if (root->output)
        handle_output_redirections(root->output, root->og_output, exec->env);
    if (root->type == CMD)
        execute_command(root, exec);
    else if (root->type == PIPE)
        execute_pipe(root, exec);
    else if (root->type == AND) {
        execute_subtree(root->l_token, exec);
        if (stat(0, 0) == 0)
            execute_subtree(root->r_token, exec);
    } else if (root->type == OR) {
        execute_subtree(root->l_token, exec);
        if (stat(0, 0) != 0)
            execute_subtree(root->r_token, exec);
    }
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdin);
    close(original_stdout);
}

void execute(t_token *token, t_exec *exec) {
    if (!token)
        return;
    if (token->input || token->output)
        execute_subtree(token, exec);
    else if (token->type == CMD)
        execute_command(token, exec);
    else if (token->type == PIPE)
        execute_pipe(token, exec);
    else if (token->type == AND)
        execute_and(token, exec);
    else if (token->type == OR)
        execute_or(token, exec);
}

void execute_and(t_token *node, t_exec *exec) {
    execute(node->l_token, exec);
    if (stat(0, 0) == 0)
        execute(node->r_token, exec);
}

void execute_or(t_token *node, t_exec *exec) {
    execute(node->l_token, exec);
    if (stat(0, 0) != 0)
        execute(node->r_token, exec);
}

void reset_fd(int in, int out) {
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    close(in);
    close(out);
}

// void execute_command(t_token *token, t_exec *exec)
// {
//     // exec->envp = env_to_envp(exec);
//         int in;
//         int out;
//         in = dup(STDIN_FILENO);
//         out = dup(STDOUT_FILENO);
//     char *cmd = token->cmd->cmd;
//     char **args = expander(token, exec, cmd);
//     int flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);

//     if (args[0])
//         cmd = args[0];
//     else
//         cmd = "\0";

//     if (check_builtins(cmd, token->cmd, exec, args)) {
//         free_strs(exec->envp);
//         exec->envp = NULL;  // Set envp to NULL after freeing
//         free_strs(args);
//         args = NULL;
//         reset_fd(in , out);
//         return;
//     }

//     // handle_here_doc(); // TODO
//     if (fork() == 0) {
//         handle_signals();
//         handle_redirections(token->cmd, exec->env, exec);
//         if(!token->cmd->og_tokens->og_cmd)
//         {
//             stat(0, 1);
//             exit(0);
//         }
//         if (!flag) {
//             stat(0, 1);
//             exit(0);
//         }
//         char *cmd_path = get_cmd(cmd, exec->envp);
//         // sta
//         execve(cmd_path, args, exec->envp);
//         perror("execve failed");
//         free(cmd_path);
//         free_strs(args);
//         args = NULL;
//         free_strs(exec->envp);
//         exec->envp = NULL;  // Set envp to NULL after freeing
//         exit(127);
//     }

//     int status;
//     wait(&status);
//     free_strs(args);
//     args = NULL;
//     free_strs(exec->envp);
//     exec->envp = NULL;  // Set envp to NULL after freeing
//     // free_strs(exec->to_free);
//     // exec->to_free = NULL;
//     // reset_fd(in , out);
//     stat(WEXITSTATUS(status), 1);
// }

// static void execute_left(t_token *node, int *fd, t_exec *exec) {
//     dup2(fd[1], STDOUT_FILENO);
//     close(fd[0]);
//     close(fd[1]);
//     execute(node, exec);
//     exit(0);
// }

// static void execute_right(t_token *node, int *fd, t_exec *exec) {
//     dup2(fd[0], STDIN_FILENO);
//     close(fd[0]);
//     close(fd[1]);
//     execute(node, exec);
//     exit(0);
// }

// static void execute_pipe(t_token *node, t_exec *exec)
// {
//     int status;
//     int fd[2];
//     int f1;
//     int f2;
//     if (pipe(fd) == -1)
//     {
//         perror("Pipe failed");
//         exit(EXIT_FAILURE);
//     }
//     f1 = fork();
//     if (f1 == 0)
//     {
//         close(fd[0]);
//         execute_left(node->l_token, fd, exec);
//     }
//     f2 = fork();
//     if (f2 == 0)
//     {
//         close(fd[1]);
//         execute_right(node->r_token, fd, exec);
//     }
//     close(fd[0]);
//     close(fd[1]);
//     waitpid(f1, &status,0);
//     waitpid(f2, &status,0);
//     stat(WEXITSTATUS(status), 1);
// }

// void execute_subtree(t_token *root, t_exec *exec) {
//     if (!root)
//         return;
//     int original_stdin = dup(STDIN_FILENO);
//     int original_stdout = dup(STDOUT_FILENO);
//     if (root->input)
//         handle_input_redirections(root->input, root->og_input, exec->env, exec);
//     if (root->output)
//         handle_output_redirections(root->output, root->og_output, exec->env);
//     if (root->type == CMD)
//         execute_command(root, exec);
//     else if (root->type == PIPE)
//         execute_pipe(root, exec);
//     else if (root->type == AND) {
//         execute_subtree(root->l_token, exec);
//         if (stat(0, 0) == 0)
//             execute_subtree(root->r_token, exec);
//     } else if (root->type == OR) {
//         execute_subtree(root->l_token, exec);
//         if (stat(0, 0) != 0)
//             execute_subtree(root->r_token, exec);
//     }
//     dup2(original_stdin, STDIN_FILENO);
//     dup2(original_stdout, STDOUT_FILENO);
//     close(original_stdin);
//     close(original_stdout);
// }

// void execute(t_token *token, t_exec *exec) {
//     if (!token)
//         return;
//     // if (token->input || token->output)
//     //     execute_subtree(token, exec);
//     else if (token->type == CMD)
//         execute_command(token, exec);
//     else if (token->type == PIPE)
//         execute_pipe(token, exec);
//     else if (token->type == AND)
//         execute_and(token, exec);
//     else if (token->type == OR)
//         execute_or(token, exec);
// }

// void execute_and(t_token *node, t_exec *exec) {
//     execute(node->l_token, exec);
//     if (stat(0, 0) == 0)
//         execute(node->r_token, exec);
// }

// void execute_or(t_token *node, t_exec *exec) {
//     execute(node->l_token, exec);
//     if (stat(0, 0) != 0)
//         execute(node->r_token, exec);
// }
