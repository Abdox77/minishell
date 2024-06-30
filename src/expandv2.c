/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expandv2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabou-ib <aabou-ib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 03:36:04 by aabou-ib          #+#    #+#             */
/*   Updated: 2024/06/30 04:55:30 by aabou-ib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**initialize_processed_args(char *cmd, char *og_cmd, t_env *env_list)
{
	char	**for_cmd;
	char	**processed_args;
	int		i;

	for_cmd = expand_arg_if_needed(cmd, og_cmd, env_list);
	i = 0;
	while (for_cmd && for_cmd[i])
		i++;
	processed_args = malloc_with_error(sizeof(char *) * (i + 1));
	i = 0;
	while (for_cmd && for_cmd[i])
	{
		processed_args[i] = strdup(for_cmd[i]);
		i++;
	}
	processed_args[i] = NULL;
	free_strs(for_cmd);
	return (processed_args);
}

char	**expand_and_concat_args(char **args, char **og_args, t_env *env_list,
		char **processed_args)
{
	char	**expanded_args;
	char	**new_processed_args;
	int		i;

	i = 1;
	while (args[i] != NULL)
	{
		expanded_args = expand_arg_if_needed(args[i], og_args[i - 1], env_list);
		new_processed_args = concat_args(processed_args, expanded_args);
		free(processed_args);
		free(expanded_args);
		processed_args = new_processed_args;
		i++;
	}
	return (processed_args);
}

char	**process_args(char **args, t_token *token, char *cmd, t_env *env_list)
{
	char	**processed_args;
	char	*og_cmd;
	char	**og_args;

	og_args = token->cmd->og_tokens->og_args;
	og_cmd = token->cmd->og_tokens->og_cmd;
	processed_args = initialize_processed_args(cmd, og_cmd, env_list);
	processed_args = expand_and_concat_args(args, og_args, env_list,
			processed_args);
	return (processed_args);
}


