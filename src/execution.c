
#include "minishell.h"

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
    // char *cmd = token->cmd->cmd;
    char *cmd = token->cmd->cmd;
    char **args = token->cmd->args;
    int flag = 1;
    expand_variables(token->cmd, exec->env, &flag);
    if(args)
        args = expand_args(args, exec->env);
    args = initialize_args_if_null(cmd, args);
    // if (check_builtins(token, exec, args))
    //     return;
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        // return -1;
    }
    else if (pid == 0)
    {
        handle_signals();
        // in child process
        // handle redirections
        handle_redirections(token->cmd);
        if(!flag)
        {
            stat(0, 1);
            exit(0);
        }
        else
        {
            if (check_builtins(token, exec, args))
                return;
            else
            {
                char *cmd_path = get_cmd(cmd, exec->envp);
                // if (cmd_path == NULL)
                // {
                //     perror(cmd);
                //     exit(127);
                // }

                execve(cmd_path, args, exec->envp);
                perror(cmd);
                stat(127, 1);
                exit(127);
            }
        }
    }
    else
    {
        // in parent process, wait for the child process to finish
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
    execute(node, exec); // execute the left command
    exit(0);
}

static void execute_right(t_token *node, int *fd, t_exec *exec)
{
    dup2(fd[0], STDIN_FILENO); // redirect stdin to read end of the pipe
    close(fd[0]); // close the read end of the pipe in child process
    close(fd[1]); // close the write end of the pipe
    execute(node, exec); // execute the right command
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
        // in child process for left command
        close(fd[0]); // close read end of the pipe
        execute_left(node->l_token, fd, exec);
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("Fork failed for right command");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) {
        // in child process for right command
        close(fd[1]); // close write end of the pipe
        execute_right(node->r_token, fd, exec);
    }

    // in parent process
    close(fd[0]);
    close(fd[1]);

    // wait for both child processes to finish
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
    else
    {
        // unknown token type
        printf("Unknown token type\n");
        status = -1;
    }

    // return the final status code
    // return status;
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
