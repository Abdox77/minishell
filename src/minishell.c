/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amohdi <amohdi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 22:21:41 by amohdi            #+#    #+#             */
/*   Updated: 2024/07/01 23:15:23 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell(t_token **head_tokens, char **line,
		char **og_line, t_exec *exec)
{
	special_trim(line);
	(*head_tokens) = lexer_manager(line);
	evaluate_syntax(*head_tokens);
	heredoc_to_fds(head_tokens);
	if (ft_print_error(NULL, NULL, RETRIEVE) == TRUE)
	{
		ft_print_error(NULL, NULL, PRINT);
		heredoc_to_fd_to_infiles(head_tokens, TRUE);
	}
	else
		heredoc_to_fd_to_infiles(head_tokens, FALSE);
	if (ft_print_error(NULL, NULL, RETRIEVE) == FALSE)
		execute(*head_tokens, exec);
	cleanup(*head_tokens);
	ft_print_error(NULL, NULL, RESET);
	free(*og_line);
	*head_tokens = NULL;
}

void	minishell_loop(char **env)
{
	char	*line;
	char	*og_line;
	t_exec	exec;
	t_token	*head_tokens;

	exec.envp = NULL;
	exec.env = parse_env(env);
	head_tokens = NULL;
	while (42)
	{
		singal_handler();
		og_line = readline("minishell$ ");
		line = og_line;
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line && *line)
			add_history(line);
		minishell(&head_tokens, &line, &og_line, &exec);
	}
	free_strs(exec.envp);
	exec.envp = NULL;
	free_env_list(exec.env);
}

int	main(int ac, char **av, char **env)
{
	(void)av;
	if (ac == 1)
		minishell_loop(env);
}
