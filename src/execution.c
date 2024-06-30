#include "minishell.h"

static char *find_env_value(const char *var_name, t_env *env_list)
{
    while (env_list != NULL)
    {
        if (strcmp(var_name, env_list->key) == 0)
            return (env_list->value);
        env_list = env_list->next;
    }
    return (NULL);
}

static size_t get_var_length(const char **str_ptr, t_env *env_list)
{
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
    while (**str_ptr && **str_ptr != '"')
    {
        if (**str_ptr == '$')
            length += get_var_length(str_ptr, env_list);
        else
        {
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

    while (*str)
    {
        if (*str == '$')
            length += get_var_length(&str, env_list);
        else if (*str == '"')
            length += get_quoted_length(&str, env_list);
        else
        {
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
    if (var_value)
    {
        ft_strcpy(*result_ptr, var_value);
        *result_ptr += strlen(var_value);
    }
}

static void append_quoted_value(char **result_ptr, const char **str_ptr, t_env *env_list)
{
    (*str_ptr)++;
    while (**str_ptr && **str_ptr != '"')
    {
        if (**str_ptr == '$')
            append_var_value(result_ptr, str_ptr, env_list);
        else
        {
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
        return (1);
    if (is_quoted(cmd))
        return (1);
    i++; // Skip the initial $
    if (!is_expandable_char(cmd[i]))
        return ( 1);
    while (cmd[i]) {
        if (!is_expandable_char(cmd[i]))
            return ( 1);
        cmd++;
    }
    char *xp = expand_string1(cmd, env_list);
    if (xp)
        return (free(xp), 1);
    return (free(xp), 0);
}

char **initialize_args_if_null(char *cmd, char **args)
{
    static char *default_args[2];

    if (args == NULL)
    {
        default_args[0] = cmd;
        default_args[1] = NULL;
        args = default_args;
    }
    return (args);
}

static void handle_signals(void)
{
    signal(SIGQUIT, SIG_DFL);
    signal(SIGINT, SIG_DFL);
}
static void handle_signals_before(void)
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

char **expander(t_token *token, t_exec *exec, char *cmd)
{
    char **args;
    char **processed_args;
    char **expanded_wildcards;

    args = initialize_args_if_null(cmd, token->cmd->args);
    processed_args = process_args(args, token->cmd->og_tokens->og_args, token->cmd->og_tokens->og_cmd, cmd, exec->env);
    expanded_wildcards = expand_wildcards(processed_args);
    free_strs(processed_args);
    // free_strs(args);
    return expanded_wildcards;
}

static void cleanup_exec(t_exec *exec, char **args, int in, int out)
{
    free_strs(exec->envp);
    exec->envp = NULL;
    free_strs(args);
    reset_fd(in, out);
}
int    is_dir(char *path)
{
    struct stat    buf;

    if (stat(path, &buf) == -1)
        return (0);
    else if (buf.st_mode & S_IFDIR)
        return (1);
    return (0);
}

static int handle_fork_execution(t_token *token, t_exec *exec, char *cmd, char **args, int flag)
{
    char *cmd_path;
    int ex;

    handle_signals();
    handle_redirections(token->cmd->redir,token->cmd->og_tokens->og_redir, exec->env, exec);
    if (!flag || !token->cmd->og_tokens->og_cmd)
    {
        stat_handler(0, 1);
        exit(0);
    }
    cmd_path = get_cmd(cmd, exec->envp);
    execve(cmd_path, args, exec->envp);
    ex = 127;
    if (access(cmd_path, F_OK) == -1)
       ex = 127; // Command not found
    else if (access(cmd_path, X_OK) == -1)
        ex = 126; // Permission denied
    if (is_dir(cmd_path))
        ex = 126;
    exec_error(cmd, cmd_path);
    free(cmd_path);
    free_strs(args);
    free_strs(exec->envp);
    exit(ex);
}

void exec_error(char *cmd, char *cmd_path)
{
    ft_write("minishell: ", 2, 0);
	ft_write(cmd, 2, 0);
	ft_write(": ", 2, 0);
    if(!*cmd)
        ft_write("command not found\n", 2, 0);
    else if (is_dir(cmd_path))
    {
        ft_write("Is a directory\n", 2, 0);
    }
    else
    {
        if (access(cmd_path, F_OK) == -1)
        {
            if(!ft_strchr(cmd, '/'))
                ft_write("command not found\n", 2, 0);
            else
                ft_write("No such file or directory\n", 2, 0);
        }
        else if (access(cmd_path, X_OK) == -1)
            ft_write("Permission denied\n", 2, 0);
    }
}

static int handle_builtin_or_fork(t_token *token, t_exec *exec, char *cmd, char **args, int flag)
{
    int status;

    if (check_builtins(cmd, token->cmd, exec, args))
        return stat_handler(0, 0);
    else
    {
        if (fork() == 0)
            handle_fork_execution(token, exec, cmd, args, flag);
        wait(&status);
        if (WIFSIGNALED(status))
        {
            if (WTERMSIG(status) == SIGINT)
                write(1, "\n", 1);
            else if (WTERMSIG(status) == SIGQUIT)
                write(1, "Quit (core dumped)\n", 20);
            return 128 + WTERMSIG(status);
        }
        return WEXITSTATUS(status);
    }
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
    handle_signals_before();
    in = dup(STDIN_FILENO);
    out = dup(STDOUT_FILENO);
    cmd = token->cmd->cmd;
    args = expander(token, exec, cmd);
    flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);
    if (args[0])
        cmd = args[0];
    else
        cmd = "\0";
    if (!*args && token->cmd->cmd)
        status = 0;
    else
        status = handle_builtin_or_fork(token, exec, cmd, args, flag);
    cleanup_exec(exec, args, in, out);
    return stat_handler(status, 1), status;
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
    stat_handler(WEXITSTATUS(status2), 1);
}

void execute_subtree(t_token *root, t_exec *exec)
{
    if (!root)
        return;
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    if (root)
        handle_redirections(root->redir,root->og_redir, exec->env, exec);
    if (root->type == CMD)
        execute_command(root, exec);
    else if (root->type == PIPE)
        execute_pipe(root, exec);
    else if (root->type == AND) {
        execute_subtree(root->l_token, exec);
        if (stat_handler(0, 0) == 0)
            execute_subtree(root->r_token, exec);
    } else if (root->type == OR) {
        execute_subtree(root->l_token, exec);
        if (stat_handler(0, 0) != 0)
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
    if (token->redir)
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

void execute_and(t_token *node, t_exec *exec)
{
    execute(node->l_token, exec);
    if (stat_handler(0, 0) == 0)
        execute(node->r_token, exec);
}

void execute_or(t_token *node, t_exec *exec)
{
    execute(node->l_token, exec);
    if (stat_handler(0, 0) != 0)
        execute(node->r_token, exec);
}

void reset_fd(int in, int out)
{
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    close(in);
    close(out);
}
