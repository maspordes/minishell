/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline_utils.h                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 15:46:08 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 15:55:09 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_PIPELINE_UTILS_H
# define EXECUTOR_PIPELINE_UTILS_H

# include "../../includes/minishell.h"

/* Pipeline data structure */
typedef struct s_pipeline_data
{
	pid_t	*pids;
	int		stdin_backup;
	int		stdout_backup;
	int		exit_status;
}	t_pipeline_data;

/* Pipeline execution functions */
int		handle_pipeline_execution(t_cmd *cmd_list, t_env **env_list,
			t_shell *shell, t_pipeline_data *data);
int		process_commands(t_cmd *cmd_list, t_env **env_list, t_shell *shell,
			pid_t *pids);
int		handle_fork_error(pid_t *pids, int *exit_status, int *pipeline_error);

/* Pipeline utility functions */
void	execute_child_command(t_cmd *cmd, t_env **env_list, t_shell *shell);
void	setup_pipe_redirections(t_cmd *current, t_cmd *prev);
void	close_unused_pipes(t_cmd *current, t_cmd *all_cmds, t_cmd *prev);
void	close_previous_pipes(t_cmd *prev);

#endif 