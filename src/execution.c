// #include "minishell.h"

// // static void handle_redirections_exec(t_redir *redir)
// // {
// //     while (redir)
// // 	{
// //         int fd = -1;
// //         if (redir->mode == INFILE || redir->mode == HEREDOC)
// // 		{
// //             fd = open(redir->file_name, O_RDONLY);
// //             if (fd < 0)
// // 			{
// //                 perror("open input file");
// //                 exit(EXIT_FAILURE);
// //             }
// //             dup2(fd, STDIN_FILENO);
// //             close(fd);
// //         }
// // 		else if (redir->mode == OUTFILE || redir->mode == APPEND)
// // 		{
// //             int flags = O_WRONLY | O_CREAT;
// //             if (redir->mode == APPEND)
// //                 flags |= O_APPEND;
// //             else
// //                 flags |= O_TRUNC;
// //             fd = open(redir->file_name, flags, 0644);
// //             if (fd < 0)
// // 			{
// //                 perror("open output file");
// //                 exit(EXIT_FAILURE);
// //             }
// //             dup2(fd, STDOUT_FILENO);
// //             close(fd);
// //         }
// //         redir = redir->next;
// //     }
// // }

// static void	execute_left(t_token *node, int *fd, char **envp)
// {
// 	dup2(fd[1], STDOUT_FILENO);
// 	close(fd[1]);
// 	// close(fd[0]);
// 	execute(node, envp);
// 	exit(127);
// 	// exit(); not sure
// }

// static void	execute_right(t_token *node, int *fd, char **envp)
// {
// 	// (void) envp;
// 	dup2(fd[0], STDIN_FILENO);
// 	close(fd[0]);
// 	// close(fd[1]);
// 	execute(node, envp);
// 	exit(100);
// 	// exit();   not sure
// }

// static void execute_pipe(t_token *node, char **envp)
// {
//     int		fd[2];
// 	pid_t 	pid1;
// 	pid_t 	pid2;
// 	int		status;

// 	if (pipe(fd) == -1)
// 	{
// 		perror("");
// 		exit(1);
// 	}
// 	pid1 = fork();
// 	if (pid1 == -1)
// 	{
// 		perror("");
// 		exit(1); // handle error
// 	}
// 	if (pid1 == 0)
// 	{
// 		close(fd[0]);
// 		execute_left(node->l_token, fd, envp);
// 	}
// 	pid2 = fork();
// 	if (pid2 == -1)
// 	{
// 		perror("");
// 		exit(1); // handle error
// 	}
// 	if (pid2 == 0)
// 	{
// 		close(fd[1]);
// 		execute_right(node->r_token, fd, envp);
// 	}
// 	close(fd[0]);
// 	close(fd[1]);
// 	waitpid(pid1, &status, WNOHANG);
// 	waitpid(pid2, &status, 0);
// 	// exit_status = WEXITSTATUS(status);    need to do this later
// }

// static int	execute_and(t_token *node, char **envp)
// {
// 	int	stat;
// 	// int	old_stdin;
// 	// int	old_stdout;

// 	// old_stdin = dup(node->cmd->redir->file_name);
// 	// old_stdout = dup(STDOUT_FILENO);
// 	// if (node->cmd->redir->mode == HEREDOC)
// 	// 	dup2(node->cmd->redir->fd, STDIN_FILENO);         //NEEED to implement fd  for here doc
// 	// else
// 	// 	dup2(node->input, STDIN_FILENO);
// 	// dup2(node->output, STDOUT_FILENO);
// 	stat = execute(node->l_token, envp);
// 	if (stat == 0)
// 		execute(node->r_token, envp);
// 	// dup2(old_stdin, STDIN_FILENO);
// 	// close(old_stdin);
// 	// dup2(old_stdout, STDOUT_FILENO);
// 	// close(old_stdout);
// 	return (1);
// }

// int	execute_or(t_token *node, char **envp)
// {
// 	int	stat;
// 	// int	old_stdin;
// 	// int	old_stdout;

// 	// old_stdin = dup(node->input);
// 	// old_stdout = dup(STDOUT_FILENO);
// 	// if (node->cmd->redir->mode == HEREDOC)
// 	// 	dup2(node->cmd->redir->fd, STDIN_FILENO);         //NEEED to implement fd  for here doc
// 	// else
// 	// 	dup2(node->input, STDIN_FILENO);
// 	// dup2(node->output, STDOUT_FILENO);
// 	stat = execute(node->l_token, envp);
// 	if (stat != 0)
// 		execute(node->r_token, envp);
// 	// dup2(old_stdin, STDIN_FILENO);
// 	// close(old_stdin);
// 	// dup2(old_stdout, STDOUT_FILENO);
// 	// close(old_stdout);
// 	return (1);
// }

// int execute(t_token *token, char **envp)
// {
//     if (token->type == CMD)
//         return execute_command(token, envp);
//     else if (token->type == PIPE)
//         execute_pipe(token, envp); //might need to make it return
//     else if (token->type == AND)
//         return execute_and(token, envp);
//     else if (token->type == OR)
//         return execute_or(token, envp);
//     return 0;
// }

// int execute_command(t_token *token, char **envp)
// {
//     // handle_redirections_exec(token->cmd->redir);
    
//     // if (check_builtins(token->cmd)) {   //need to implement later
//     //     return 0;
//     // }
//     char *cmd;
//     pid_t pid = fork();
//     if (pid == 0)
//     {
// 		// handle_redirections_exec(token->cmd->redir);
//         cmd = get_cmd(token->cmd->cmd, envp);
// 		// printf("rhis is cmd%s,\n this is args %s, heeeeeere %s \n" ÷, token->cmd->cmd , token->cmd->args[0], envp[5]);
//         execve(cmd ,token->cmd->args, envp);
//         perror("");
//         exit(127);
//     }
//     else if (pid > 0)
//     {
//         int status;
//         waitpid(pid, &status, 0);
//         return WEXITSTATUS(status);
//     }
//     else
//     {
//         perror("fork failed");
//         return -1;
//     }
// }



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


// static void handle_redirections_exec(t_redir *redir)
// {
//     while (redir)
//     {
//         int fd = -1;

//         if (redir->mode == INFILE)
//         {
//             fd = open(redir->file_name, O_RDONLY);
//             if (fd < 0)
//             {
//                 perror("Failed to open input file");
//                 return;  
//             }
//             dup2(fd, STDIN_FILENO);
//             close(fd);
//         }
//         // output redirection (overwrite)
//         else if (redir->mode == OUTFILE)
//         {
//             fd = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (fd < 0)
//             {
//                 perror("Failed to open output file");
//                 return;
//             }
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//         else if (redir->mode == APPEND)
//         {
//             fd = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             if (fd < 0)
//             {
//                 perror("Failed to open output file");
//                 return; 
//             }
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//         redir = redir->next;
//     }
// }

void execute_command(t_token *token, t_exec *exec) {
    char *cmd = token->cmd->cmd;
    char **args = token->cmd->args;

    if (check_builtins(token, exec))
        return;
    args = initialize_args_if_null(cmd, args);
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        // return -1;
    }
    else if (pid == 0)
    {
        // in child process
        // handle redirections
        handle_redirections(token->cmd);
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
    else
    {
        // in parent process, wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);
        stat(WEXITSTATUS(status), 1);
    }
}

static void execute_left(t_token *node, int *fd, t_exec *exec) {
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]); // close the read end of the pipe
    close(fd[1]); // close the write end of the pipe in child process
    execute(node, exec); // execute the left command
    exit(0);
}

static void execute_right(t_token *node, int *fd, t_exec *exec) {
    dup2(fd[0], STDIN_FILENO); // redirect stdin to read end of the pipe
    close(fd[0]); // close the read end of the pipe in child process
    close(fd[1]); // close the write end of the pipe
    execute(node, exec); // execute the right command
    exit(0);
}

static void execute_pipe(t_token *node, t_exec *exec) {
    int fd[2];
    pid_t pid1, pid2;
    int status1, status2;

    if (pipe(fd) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("Fork failed for left command");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
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
    // if (token == NULL)
    //     return 0;
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
