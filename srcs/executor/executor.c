/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/07/19 18:15:00 by marrey           ###   ########.fr       */
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

static int	preprocess_heredoc(t_redirect *redirect)
{
	int		fd;
	char	*temp_file;
	int		temp_fd;
	char	buffer[1024];
	ssize_t	bytes_read;

	fd = handle_heredoc(redirect->file);
	if (fd == -1)
		return (-1);
	temp_file = ft_strdup("/tmp/heredoc_XXXXXX");
	if (!temp_file)
	{
		close(fd);
		return (-1);
	}
	temp_fd = mkstemp(temp_file);
	if (temp_fd == -1)
	{
		close(fd);
		free(temp_file);
		return (-1);
	}
	while (1)
	{
		bytes_read = read(fd, buffer, sizeof(buffer));
		if (bytes_read <= 0)
			break ;
		if (write(temp_fd, buffer, bytes_read) != bytes_read)
		{
			close(fd);
			close(temp_fd);
			unlink(temp_file);
			free(temp_file);
			return (-1);
		}
	}
	close(fd);
	close(temp_fd);
	free(redirect->file);
	redirect->file = temp_file;
	redirect->type = T_REDIR_IN;
	return (0);
}

static void	cleanup_temp_files(t_cmd *cmd_list)
{
	t_cmd		*current;
	t_redirect	*redirect;

	current = cmd_list;
	while (current)
	{
		redirect = current->redirects;
		while (redirect)
		{
			if (redirect->type == T_REDIR_IN && redirect->file
				&& ft_strncmp(redirect->file, "/tmp/heredoc_", 13) == 0)
			{
				unlink(redirect->file);
			}
			redirect = redirect->next;
		}
		current = current->next;
	}
}

static int	preprocess_heredocs_in_cmd(t_cmd *cmd)
{
	t_redirect	*redirect;

	redirect = cmd->redirects;
	while (redirect)
	{
		if (redirect->type == T_HEREDOC)
		{
			if (preprocess_heredoc(redirect) == -1)
				return (-1);
		}
		redirect = redirect->next;
	}
	return (0);
}

static int	preprocess_all_heredocs(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (preprocess_heredocs_in_cmd(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}

static void	execute_child_process(t_cmd *current, t_cmd *prev,
		t_exec_data *data)
{
	int	exit_status;

	setup_child_pipes(current, prev, data->cmd_list);
	if (!setup_redirections(current->redirects))
		exit(1);
	if (current->args && current->args[0])
	{
		if (is_builtin(current->args[0]))
			exit_status = exec_builtin(current, data->env_list, data->shell);
		else
			exit_status = execute_external_cmd(current, *(data->env_list));
		exit(exit_status);
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
		{
			perror("fork");
			return (1);
		}
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
	int			result;

	if (preprocess_all_heredocs(cmd_list) == -1)
		return (1);
	data.cmd_list = cmd_list;
	data.env_list = env_list;
	data.shell = shell;
	num_cmds = count_commands(cmd_list);
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids || (num_cmds > 1 && !create_pipes(cmd_list)))
	{
		cleanup_temp_files(cmd_list);
		if (pids)
			free(pids);
		return (1);
	}
	if (fork_and_execute(&data, pids) != 0)
	{
		cleanup_temp_files(cmd_list);
		free(pids);
		return (1);
	}
	close_all_pipes(cmd_list);
	result = wait_for_children(pids, num_cmds);
	cleanup_temp_files(cmd_list);
	free(pids);
	return (result);
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
