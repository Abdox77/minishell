
#include "minishell.h"

// static char *find_env_value(const char *var_name, t_env *env_list) {
//     while (env_list != NULL) {
//         if (strcmp(var_name, env_list->key) == 0)
//             return env_list->value;
//         env_list = env_list->next;
//     }
//     return NULL;
// }

// static size_t get_expanded_length(const char *str, t_env *env_list)
// {
//     size_t length = 0;
//     while (*str)
//     {
//         if (*str == '$')
//         {
//             str++;
//             const char *var_start = str;
//             while (*str && (ft_isalnum(*str) || *str == '_'))
//                 str++;
//             size_t var_len = str - var_start;
//             char *var_name = ft_strndup(var_start, var_len);
//             char *var_value = find_env_value(var_name, env_list);
//             free(var_name);
//             if (var_value)
//                 length += strlen(var_value);
//         }
//         else if (*str == '"')
//         {
//             str++;
//             while (*str && *str != '"')
//             {
//                 if (*str == '$')
//                 {
//                     str++;
//                     const char *var_start = str;
//                     while (*str && (isalnum(*str) || *str == '_'))
//                         str++;
//                     size_t var_len = str - var_start;
//                     char *var_name = strndup(var_start, var_len);
//                     char *var_value = find_env_value(var_name, env_list);
//                     free(var_name);
//                     if (var_value)
//                         length += strlen(var_value);
//                 }
//                 else
//                 {
//                     length++;
//                     str++;
//                 }
//             }
//             if (*str == '"')
//                 str++;
//         }
//         else
//         {
//             length++;
//             str++;
//         }
//     }
//     return length;
// }
// static char *expand_string1(const char *str, t_env *env_list) {
//     size_t result_size = get_expanded_length(str, env_list) + 1;
//     char *result = (char *)malloc(result_size);
//     if (!result)
//     {
//         perror("malloc failed");
//         exit(EXIT_FAILURE);
//     }
//     result[0] = '\0';
//     char *result_ptr = result;

//     while (*str)
//     {
//         if (*str == '$')
//         {
//             str++;
//             const char *var_start = str;
//             while (*str && (isalnum(*str) || *str == '_'))
//                 str++;
//             size_t var_len = str - var_start;
//             char *var_name = strndup(var_start, var_len);
//             char *var_value = find_env_value(var_name, env_list);
//             free(var_name);
//             if (var_value)
//             {   
//                 strcpy(result_ptr, var_value);
//                 result_ptr += strlen(var_value);
//             }
//         }
//         else if (*str == '"')
//         {
//             str++;
//             while (*str && *str != '"')
//             {
//                 if (*str == '$')
//                 {
//                     str++;
//                     const char *var_start = str;
//                     while (*str && (isalnum(*str) || *str == '_'))
//                         str++;
//                     size_t var_len = str - var_start;
//                     char *var_name = strndup(var_start, var_len);
//                     char *var_value = find_env_value(var_name, env_list);
//                     free(var_name);
//                     if (var_value)
//                     {   
//                         strcpy(result_ptr, var_value);
//                         result_ptr += strlen(var_value);
//                     }
//                 }
//                 else
//                     *result_ptr++ = *str++;
//             }
//             if (*str == '"')
//                 str++;
//         }
//         else
//             *result_ptr++ = *str++;
//     }
//     *result_ptr = '\0';
//     return result;
// }

// static int is_expandable_char(char c)
// {
//     return (isalnum(c) || c == '_' || c== '$');
// }

// static int is_quoted(const char *str)
// {
//     int i = 0;
//     while (str[i])
//     {
//         if (str[i] == '"' || str[i] == '\'')
//             return 1;
//         i++;
//     }
//     return 0;
// }

//  int check_to_expand(char *cmd, t_env *env_list)
// {
//     (void)env_list;
//     int i = 0;
//     if (!cmd || cmd[i] != '$')
//         return (printf("because of 0; \n"),1);
    
//     if (is_quoted(cmd))
//         return (printf("ghsdldsbfldsbfljdsbflsdjbfs"),1);

//     i++; // Skip the initial $

//     if (!is_expandable_char(cmd[i]))
//         return (printf("because of 1; \n"),1);
    
//     while (cmd[i])
//     {
//         if (!is_expandable_char(cmd[i]))
//         return (printf("because of 2; \n"),1);
//         cmd++;
//     }
//     if(expand_string1(cmd, env_list)){

//             printf("ghsdldsbfldsbfljdsbflsdjbfs");
//            return (printf("because of 3; \n"),1);}
//     return 0;
// }

// static char **initialize_args_if_null(char *cmd, char **args)
// {
//     if (args == NULL)
//     {
//         args = malloc(2 * sizeof(char*));
//         if (args == NULL)
//         {
//             perror("Failed to allocate memory for args");
//             exit(EXIT_FAILURE);
//         }
//         args[0] = ft_strdup(cmd);
//         args[1] = NULL;
//     }
//     return args;
// }

#include "minishell.h"

static char *find_env_value(const char *var_name, t_env *env_list) {
    while (env_list != NULL) {
        if (strcmp(var_name, env_list->key) == 0)
            return (env_list->value);
        env_list = env_list->next;
    }
    return (NULL);
}

static size_t get_var_length(const char **str_ptr, t_env *env_list) {
    const char  *var_start;
    size_t      var_len;
    char        *var_name;
    char        *var_value;

    (*str_ptr)++;
    var_start = *str_ptr;
    while (**str_ptr && (ft_isalnum(**str_ptr) || **str_ptr == '_'))
        (*str_ptr)++;
    var_len = *str_ptr - var_start;
    var_name = ft_strndup(var_start, var_len);
    var_value = find_env_value(var_name, env_list);
    free(var_name);
    if (var_value)
        return (strlen(var_value));
    return (0);
}

static size_t get_quoted_length(const char **str_ptr, t_env *env_list)
{
    size_t length = 0;

    (*str_ptr)++;
    while (**str_ptr && **str_ptr != '"') {
        if (**str_ptr == '$') {
            length += get_var_length(str_ptr, env_list);
        } else {
            length++;
            (*str_ptr)++;
        }
    }
    if (**str_ptr == '"')
        (*str_ptr)++;
    return (length);
}

static size_t get_expanded_length(const char *str, t_env *env_list) {
    size_t length = 0;

    while (*str) {
        if (*str == '$') {
            length += get_var_length(&str, env_list);
        } else if (*str == '"') {
            length += get_quoted_length(&str, env_list);
        } else {
            length++;
            str++;
        }
    }
    return (length);
}

static void append_var_value(char **result_ptr, const char **str_ptr, t_env *env_list)
{
    const char  *var_start;
    size_t      var_len;
    char        *var_name;
    char        *var_value;

    (*str_ptr)++;
    var_start = *str_ptr;
    while (**str_ptr && (isalnum(**str_ptr) || **str_ptr == '_'))
        (*str_ptr)++;
    var_len = *str_ptr - var_start;
    var_name = strndup(var_start, var_len);
    var_value = find_env_value(var_name, env_list);
    free(var_name);
    if (var_value) {
        strcpy(*result_ptr, var_value);
        *result_ptr += strlen(var_value);
    }
}

static void append_quoted_value(char **result_ptr, const char **str_ptr, t_env *env_list)
{
    (*str_ptr)++;
    while (**str_ptr && **str_ptr != '"') {
        if (**str_ptr == '$') {
            append_var_value(result_ptr, str_ptr, env_list);
        } else {
            **result_ptr = **str_ptr;
            (*result_ptr)++;
            (*str_ptr)++;
        }
    }
    if (**str_ptr == '"')
        (*str_ptr)++;
}

static char *expand_string1(const char *str, t_env *env_list)
{
    size_t result_size;
    char   *result;
    char   *result_ptr;

    result_size = get_expanded_length(str, env_list) + 1;
    result = (char *)malloc(result_size);
    if (!result)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    result[0] = '\0';
    result_ptr = result;
    while (*str)
    {
        if (*str == '$')
            append_var_value(&result_ptr, &str, env_list);
        else if (*str == '"')
            append_quoted_value(&result_ptr, &str, env_list);
        else
            *result_ptr++ = *str++;
    }
    *result_ptr = '\0';
    return (result);
}

static int is_expandable_char(char c)
{
    return (isalnum(c) || c == '_' || c == '$');
}

static int is_quoted(const char *str)
{
    while (*str)
    {
        if (*str == '"' || *str == '\'')
            return (1);
        str++;
    }
    return (0);
}

int check_to_expand(char *cmd, t_env *env_list)
{
    int i = 0;

    (void)env_list;
    if (!cmd || cmd[i] != '$')
        return (printf("because of 0;\n"), 1);
    if (is_quoted(cmd))
        return (printf("ghsdldsbfldsbfljdsbflsdjbfs"), 1);

    i++; // Skip the initial $

    if (!is_expandable_char(cmd[i]))
        return (printf("because of 1;\n"), 1);
    while (cmd[i]) {
        if (!is_expandable_char(cmd[i]))
            return (printf("because of 2;\n"), 1);
        cmd++;
    }
    if (expand_string1(cmd, env_list))
        return (printf("because of 3;\n"), 1);
    return (0);
}

char **initialize_args_if_null(char *cmd, char **args)
{
    if (args == NULL)
    {
        args = malloc(2 * sizeof(char *));
        if (args == NULL)
        {
            perror("Failed to allocate memory for args");
            exit(EXIT_FAILURE);
        }
        args[0] = ft_strdup(cmd);
        args[1] = NULL;
    }
    return (args);
}

static void handle_signals(void)
{
    signal(SIGINT, SIG_DFL);
}

char **expander(t_token *token, t_exec *exec, char *cmd)
{
    // char **args;
    char **processed_args;
    char **expanded_wildcards;

    exec->to_free = initialize_args_if_null(cmd, token->cmd->args);
    processed_args = process_args(exec->to_free, token->cmd->og_tokens->og_args, token->cmd->og_tokens->og_cmd, cmd, exec->env);
    expanded_wildcards = expand_wildcards(processed_args);
    free_strs(processed_args);
    return expanded_wildcards;
}

// int execute_command(t_token *token, t_exec *exec) {
//     exec->envp = env_to_envp(exec);
//     int in = dup(STDIN_FILENO);
//     int out = dup(STDOUT_FILENO);

//     char *cmd = token->cmd->cmd;
//     char **args = expander(token, exec, cmd);
//     int flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);

//     if (args[0])
//         cmd = args[0];
//     else
//         cmd = "\0";

//     if (check_builtins(cmd, token->cmd, exec, args)) {
//         free_strs(exec->envp);
//         exec->envp = NULL;
//         free_strs(args);
//         args = NULL;
//         reset_fd(in, out);
//         return 0;
//     }

//     // handle_here_doc(); // TODO
//     if (fork() == 0) {
//         handle_signals();
//         handle_redirections(token->cmd, exec->env, exec);

//         if (!flag || !token->cmd->og_tokens->og_cmd) {
//             stat(0, 1);
//             exit(0);
//         }

//         char *cmd_path = get_cmd(cmd, exec->envp);
//         execve(cmd_path, args, exec->envp);
//         perror("execve failed");
//         free(cmd_path);
//         free_strs(args);
//         args = NULL;
//         free_strs(exec->envp);
//         exec->envp = NULL;
//         exit(127);
//     }

//     int status;
//     wait(&status);
//     free_strs(args);
//     args = NULL;
//     free_strs(exec->envp);
//     exec->envp = NULL;
//     reset_fd(in, out);
//     return (stat(WEXITSTATUS(status), 1), WEXITSTATUS(status));
// }


static void cleanup_exec(t_exec *exec, char **args, int in, int out)
{
    free_strs(exec->envp);
    exec->envp = NULL;
    free_strs(args);
    reset_fd(in, out);
}

static int handle_fork_execution(t_token *token, t_exec *exec, char *cmd, char **args, int flag)
{
    char *cmd_path;

    handle_signals();
    handle_redirections(token->cmd, exec->env, exec);
    if (!flag || !token->cmd->og_tokens->og_cmd)
    {
        stat(0, 1);
        exit(0);
    }
    cmd_path = get_cmd(cmd, exec->envp);
    execve(cmd_path, args, exec->envp);
    perror("execve failed");
    free(cmd_path);
    free_strs(args);
    free_strs(exec->envp);
    exit(127);
}

static int handle_builtin_or_fork(t_token *token, t_exec *exec, char *cmd, char **args, int flag)
{
    int status;

    if (check_builtins(cmd, token->cmd, exec, args))
        return 0;
    if (fork() == 0)
        handle_fork_execution(token, exec, cmd, args, flag);
    wait(&status);
    return WEXITSTATUS(status);
}

int execute_command(t_token *token, t_exec *exec)
{
    char *cmd;
    char **args;
    int flag;
    int in;
    int out;
    int status;

    exec->envp = env_to_envp(exec);
    in = dup(STDIN_FILENO);
    out = dup(STDOUT_FILENO);
    cmd = token->cmd->cmd;
    args = expander(token, exec, cmd);
    flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);
    if (args[0])
        cmd = args[0];
    else
        cmd = "\0";

    status = handle_builtin_or_fork(token, exec, cmd, args, flag);
    cleanup_exec(exec, args, in, out);
    return stat(status, 1), status;
}

static void execute_left(t_token *node, int *fd, t_exec *exec)
{
    int status;
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
    status = execute(node, exec);
    exit(status);
}

static void execute_right(t_token *node, int *fd, t_exec *exec)
{
    int status;
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);
    status = execute(node, exec);
    exit(status);
}

static void execute_pipe(t_token *node, t_exec *exec)
{
    int status1;
    int status2;
    pid_t pid[2];
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }
    pid[0] = fork();
    if (pid[0] == 0) {
        close(fd[0]);
        execute_left(node->l_token, fd, exec);
    }
    pid[1] = fork();
    if (pid[1] == 0) {
        close(fd[1]);
        execute_right(node->r_token, fd, exec);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid[0], &status1, 0);
    waitpid(pid[1], &status2, 0);
    puts("here with exit status ");
    puts(ft_itoa(WEXITSTATUS(status2)));
    stat(WEXITSTATUS(status2), 1);
}

void execute_subtree(t_token *root, t_exec *exec)
{
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

int execute(t_token *token, t_exec *exec)
{
    int status;

    status = 0;
    if (!token)
        return 0;
    if (token->input || token->output)
        execute_subtree(token, exec);
    else if (token->type == PIPE)
        execute_pipe(token, exec);
    else if (token->type == AND)
        execute_and(token, exec);
    else if (token->type == OR)
        execute_or(token, exec);
    else if (token->type == CMD)
        status = execute_command(token, exec);
    return (status);
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
