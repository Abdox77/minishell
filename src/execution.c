
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
        args[0] = cmd;
        args[1] = NULL;
    }
    return args;
}

static void handle_signals(void)
{
    signal(SIGINT, SIG_DFL);
}

void execute_command(t_token *token, t_exec *exec)
{
    exec->envp = env_to_envp(exec);
    char *cmd = token->cmd->cmd;
    char **args = token->cmd->args;
    int flag = check_to_expand(token->cmd->og_tokens->og_cmd, exec->env);
    args = initialize_args_if_null(cmd, args);
    args = process_args(args, token->cmd->og_tokens->og_args, token->cmd->og_tokens->og_cmd, cmd, exec->env);
    args = expand_wildcards(args);
    if(args)
        cmd = args[0];
    else
        cmd = "";
    if (check_builtins(token, exec, args))
        return;
    pid_t pid = fork();
    if (pid < 0)
        perror("Fork failed");
    else if (pid == 0)
    {
        handle_signals();
        handle_redirections(token->cmd, exec->env);
    printf("flag is ________________--%d================== \n ", flag); /////////////////////////////////now i need you to create an another thing please which is i.e     args[0]=ls args[1]=*.c args[2]=-la (the directory has test.c and 1.c); you give a 2d array args[0]=ls args[1]=test.c ags[2]=1.c args[3]=-la i hope you understand

        if(!flag)
        {
            stat(0, 1);
            exit(0);
            return;
        }
            printf("args are========%s \n", args[0]);
            // else
            // {
                char *cmd_path = get_cmd(cmd, exec->envp);
                printf("===============executing======= %s \n \n", cmd_path);
                execve(cmd_path, args, exec->envp);
                ft_write(cmd,2,0);
                ft_write(": command not found", 2, 1);
                stat(127, 1);
                exit(127);
            // }
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        stat(WEXITSTATUS(status), 1);
    }
}

static void execute_left(t_token *node, int *fd, t_exec *exec)
{
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]); // close the read end of the pipe
    close(fd[1]); // close the write end of the pipe in child process
    execute(node, exec);
    exit(0);
}

static void execute_right(t_token *node, int *fd, t_exec *exec)
{
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]); // close the read end of the pipe in child process
    close(fd[1]); // close the write end of the pipe
    execute(node, exec);
    exit(0);
}

static void execute_pipe(t_token *node, t_exec *exec)
{
    int fd[2];
    pid_t pid1, pid2;
    int status1, status2;

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("Fork failed for left command");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0)
    {
        close(fd[0]); // close read end of the pipe
        execute_left(node->l_token, fd, exec);
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("Fork failed for right command");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        close(fd[1]);
        execute_right(node->r_token, fd, exec);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    stat(WEXITSTATUS(status2), 1);
}

void execute(t_token *token, t_exec *exec)
{
    if (token == NULL)
        return;
    int status = 0;
    (void)status;
    if (token->type == CMD)
        // execute a simple command
        execute_command(token, exec);
    else if (token->type == PIPE)
        execute_pipe(token, exec);
    else if (token->type == AND)
    {
       execute(token->l_token, exec);
        if (stat(0, 0) == 0)
            execute(token->r_token, exec);
    }
    else if (token->type == OR)
    {
        execute(token->l_token, exec);
        if (stat(0, 0) != 0)
            execute(token->r_token, exec);
    }
}


void execute_and(t_token *node, t_exec *exec)
{
    execute(node->l_token, exec);
    if (stat(0, 0) == 0)
        execute(node->r_token, exec);
    // return status;
}

void execute_or(t_token *node, t_exec *exec)
{
    execute(node->l_token, exec);
    if (stat(0, 0) != 0)
        execute(node->r_token, exec);
    // return status;
}
