#include "minishell.h"

static void	execute_left(t_token *node, int *fd, char **envp)
{
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	execute(node, envp);
	// exit(); not sure
}

static void	execute_right(t_token *node, int *fd, char **envp)
{

	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
	// exit();   not sure
}

static void execute_pipe(t_token *node, char **envp)
{
    int		fd[2];
	pid_t 	pid1;
	pid_t 	pid2;
	int		status;

	if (pipe(fd) == -1)
	{
		perror("");
		exit(1);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("");
		exit(1); // handle error
	}
	if (pid1 == 0)
		execute_left(node->l_token, fd, envp);
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("");
		exit(1); // handle error
	}
	if (pid2 == 0)
		execute_right(node->r_token, fd, envp);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	// exit_status = WEXITSTATUS(status);    need to do this later
}

static int	execute_and(t_token *node, char **envp)
{
	int	stat;
	int	old_stdin;
	int	old_stdout;

	old_stdin = dup(node->cmd->redir->file_name);
	old_stdout = dup(STDOUT_FILENO);
	// if (node->cmd->redir->mode == HEREDOC)
	// 	dup2(node->cmd->redir->fd, STDIN_FILENO);         //NEEED to implement fd  for here doc
	// else
		dup2(node->input, STDIN_FILENO);
	dup2(node->output, STDOUT_FILENO);
	stat = execute(node->l_token, envp);
	if (stat == 0)
		execute(node->r_token, envp);
	dup2(old_stdin, STDIN_FILENO);
	close(old_stdin);
	dup2(old_stdout, STDOUT_FILENO);
	close(old_stdout);
	return (1);
}

int	execute_or(t_token *node, char **envp)
{
	int	stat;
	int	old_stdin;
	int	old_stdout;

	old_stdin = dup(node->input);
	old_stdout = dup(STDOUT_FILENO);
	// if (node->cmd->redir->mode == HEREDOC)
	// 	dup2(node->cmd->redir->fd, STDIN_FILENO);         //NEEED to implement fd  for here doc
	// else
		dup2(node->input, STDIN_FILENO);
	dup2(node->output, STDOUT_FILENO);
	stat = execute(node->l_token, envp);
	if (stat != 0)
		execute(node->r_token, envp);
	dup2(old_stdin, STDIN_FILENO);
	close(old_stdin);
	dup2(old_stdout, STDOUT_FILENO);
	close(old_stdout);
	return (1);
}

int execute(t_token *token, char **envp)
{
    if (token->type == CMD)
        return execute_command(token, envp);
    else if (token->type == PIPE)
        execute_pipe(token, envp); //might need to make it return
    else if (token->type == AND)
        return execute_and(token, envp);
    else if (token->type == OR)
        return execute_or(token, envp);
    return 0;
}

int execute_command(t_token *token, char **envp)
{
    handle_redirections_exec(token->cmd->redir);
    
    // if (check_builtins(token->cmd)) {   //need to implement later
    //     return 0;
    // }
    char *cmd;
    pid_t pid = fork();
    if (pid == 0)
    {
        cmd = get_cmd(token->cmd->cmd, envp);
        execve(cmd, token->cmd->args, envp);
        perror("execve failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    else
    {
        perror("fork failed");
        return -1;
    }
}
