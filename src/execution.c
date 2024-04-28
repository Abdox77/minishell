#include "minishell.h"

void execute_command(t_token *token)
{
	open_infiles_outfile(token->cmd->redir); // not sure about the name here
	execve(token->cmdi);
}

void open_pipe_and_redirect(void)
{
	int fd[2];
	pipe(fd);
}

void execute_commands(t_token *token)
{
	pid_t pid;

	if (!token)
		printf("how is token null in execute command\n");
	open_pipe_and_redirect();
	pid = fork();
	if (pid == 0)
		execute_command(token);
	else
		return ;
}
void execute_manager(t_token *head)
{
	if (!head)
		return ;
	execute_command(head->l_token);
	execute_manager(head->right);
}
