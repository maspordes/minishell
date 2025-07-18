/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/17 18:47:42 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	expand_all_commands(t_shell *shell)
{
	t_cmd	*current_cmd;

	current_cmd = shell->cmd_list;
	while (current_cmd)
	{
		if (expand_command(current_cmd, shell) != 0)
		{
			shell->exit_status = 1;
			return (1);
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}

int	execute_pipeline(t_shell *shell)
{
	int	expansion_failed;

	expansion_failed = expand_all_commands(shell);
	if (!expansion_failed)
	{
		shell->exit_status = executor(shell->cmd_list,
				&(shell->env_list), shell);
	}
	free_cmds(shell->cmd_list);
	shell->cmd_list = NULL;
	return (shell->exit_status);
}

int	process_input(t_shell *shell)
{
	if (!shell->input || shell->input[0] == '\0')
		return (0);
	shell->tokens = lexer(shell->input);
	if (!shell->tokens)
		return (0);
	shell->cmd_list = parser(shell->tokens);
	free_tokens(shell->tokens);
	shell->tokens = NULL;
	if (!shell->cmd_list)
		return (0);
	return (execute_pipeline(shell));
}
