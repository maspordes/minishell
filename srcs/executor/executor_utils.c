/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 18:43:54 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_single_parent_builtin(t_cmd *cmd_list, int num_cmds)
{
	return (num_cmds == 1 && cmd_list->args && cmd_list->args[0]
		&& is_parent_builtin(cmd_list->args[0]));
}

static int	execute_single_builtin(t_cmd *cmd, t_env **env_list, t_shell *shell)
{
	int	exit_status;
	int	stdin_backup;
	int	stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
		return (1);
	if (!setup_redirections(cmd->redirects))
		exit_status = 1;
	else if (!cmd->args || !cmd->args[0])
		exit_status = 0;
	else
	{
		exit_status = exec_builtin(cmd, env_list, shell);
		if (cmd->args[0] && ft_strcmp(cmd->args[0], "exit") == 0)
		{
			shell->should_exit = 1;
			shell->exit_status = exit_status;
		}
	}
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	return (exit_status);
}

int	execute_builtin_command(t_cmd *cmd_list, t_env **env_list, t_shell *shell)
{
	return (execute_single_builtin(cmd_list, env_list, shell));
}
