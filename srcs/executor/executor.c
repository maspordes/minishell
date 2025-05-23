/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 22:38:57 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		create_pipes(t_cmd *cmd_list);
void	close_all_pipes(t_cmd *cmd_list);
int		count_commands(t_cmd *cmd_list);
int		setup_redirections(t_redirect *redirects);
int		execute_external_cmd(t_cmd *cmd, t_env *env_list);
int		is_parent_builtin(const char *cmd_name);
void	setup_child_pipes(t_cmd *current, t_cmd *prev, t_cmd *cmd_list);
int		wait_for_children(pid_t *pids, int num_cmds);

static void	execute_child_process(t_cmd *current, t_cmd *prev,
		t_exec_data *data)
{
	setup_child_pipes(current, prev, data->cmd_list);
	if (!setup_redirections(current->redirects))
		exit(1);
	if (current->args && current->args[0])
	{
		if (is_builtin(current->args[0]))
			exit(exec_builtin(current, data->env_list, data->shell));
		else
			exit(execute_external_cmd(current, *(data->env_list)));
	}
	exit(0);
}

static int	fork_and_execute(t_exec_data *data, pid_t *pids)
{
	t_cmd	*current;
	t_cmd	*prev;
	int		i;

	current = data->cmd_list;
	prev = NULL;
	i = 0;
	while (current)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			execute_child_process(current, prev, data);
		else if (pids[i] < 0)
			return (perror("fork"), 1);
		if (prev && prev->pipe_fd[0] != -1)
			close(prev->pipe_fd[0]);
		if (prev && prev->pipe_fd[1] != -1)
			close(prev->pipe_fd[1]);
		prev = current;
		current = current->next;
		i++;
	}
	return (0);
}

static int	execute_cmd_pipeline(t_cmd *cmd_list, t_env **env_list,
		t_shell *shell)
{
	pid_t		*pids;
	int			num_cmds;
	t_exec_data	data;

	data.cmd_list = cmd_list;
	data.env_list = env_list;
	data.shell = shell;
	num_cmds = count_commands(cmd_list);
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids || (num_cmds > 1 && !create_pipes(cmd_list)))
		return (1);
	if (fork_and_execute(&data, pids) != 0)
		return (1);
	close_all_pipes(cmd_list);
	return (wait_for_children(pids, num_cmds));
}

int	executor(t_cmd *cmd_list, t_env **env_list, t_shell *shell)
{
	int		exit_status;
	int		num_cmds;
	char	*exit_str;

	if (!cmd_list || !shell)
		return (0);
	num_cmds = count_commands(cmd_list);
	if (is_single_parent_builtin(cmd_list, num_cmds))
		exit_status = execute_builtin_command(cmd_list, env_list, shell);
	else
		exit_status = execute_cmd_pipeline(cmd_list, env_list, shell);
	exit_str = ft_itoa(exit_status);
	if (exit_str)
	{
		set_env_value(env_list, "?", exit_str);
		free(exit_str);
	}
	shell->exit_status = exit_status;
	return (exit_status);
}
