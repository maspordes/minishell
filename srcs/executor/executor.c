/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/06 21:26:57 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 检查是否为内置命令 */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

/* 执行内置命令 */
int	exec_builtin(t_cmd *cmd, t_env **env_list, t_shell *shell)
{
	if (!cmd->args || !cmd->args[0])
		return (0);

	// 确保 shell 非空
	if (!shell)
	{
		fprintf(stderr, "Shell structure is NULL in exec_builtin\n");
		return (1);
	}

	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(cmd->args, *env_list));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(cmd->args, env_list));
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		return (ft_export(cmd->args, env_list));
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (ft_unset(cmd->args, env_list));
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(*env_list));
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (ft_exit(cmd->args, shell));
	return (0);
}

/* 创建管道 */
static int	create_pipes(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current && current->next)
	{
		if (pipe(current->pipe_fd) == -1)
		{
			perror("pipe");
			return (0);
		}
		current = current->next;
	}
	return (1);
}

/* 设置文件重定向 */
static int	setup_redirections(t_redirect *redirects)
{
	int	fd;

	while (redirects)
	{
		if (redirects->type == T_REDIR_IN)
		{
			fd = open(redirects->file, O_RDONLY);
			if (fd == -1)
			{
				perror(redirects->file);
				return (0);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redirects->type == T_REDIR_OUT)
		{
			fd = open(redirects->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(redirects->file);
				return (0);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redirects->type == T_REDIR_APPEND)
		{
			fd = open(redirects->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(redirects->file);
				return (0);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redirects->type == T_HEREDOC)
		{
			fd = handle_heredoc(redirects->file);
			if (fd == -1)
				return (0);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		redirects = redirects->next;
	}
	return (1);
}

/* 查找可执行文件的路径 */
static char	*find_executable(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*cmd_path;
	int		i;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd);
		if (access(cmd_path, X_OK) == 0)
		{
			free_array(paths);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

/* 释放字符串数组 */
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/* 执行外部命令 */
static int	execute_external_cmd(t_cmd *cmd, t_env *env_list)
{
	char	*cmd_path;
	char	**env_array;
	pid_t	pid;
	int		status;

	cmd_path = find_executable(cmd->args[0], env_list);
	if (!cmd_path)
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		return (127);
	}

	env_array = env_to_array(env_list);

	reset_signals();

	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, cmd->args, env_array) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		free_array(env_array);
		return (1);
	}

	waitpid(pid, &status, 0);

	free(cmd_path);
	free_array(env_array);
	setup_signals();

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/* 执行命令链表 */
static int	execute_commands(t_cmd *cmd_list, t_env **env_list, t_shell *shell)
{
	t_cmd	*current;
	t_cmd	*prev;
	int		exit_status = 0;
	int		stdin_backup;
	int		stdout_backup;
	pid_t	*pids;
	int		i;
	int		num_cmds;

	// Count number of commands
	num_cmds = 0;
	current = cmd_list;
	while (current)
	{
		num_cmds++;
		current = current->next;
	}

	// Allocate array for process IDs
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
		return (1);

	if (!create_pipes(cmd_list))
	{
		free(pids);
		return (1);
	}

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	// Execute each command in its own process
	current = cmd_list;
	prev = NULL;
	i = 0;
	while (current)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			// Child process
			// Close all pipe ends in child process
			t_cmd *temp = cmd_list;
			while (temp)
			{
				if (temp != current && temp != prev)
				{
					if (temp->pipe_fd[0] != -1)
						close(temp->pipe_fd[0]);
					if (temp->pipe_fd[1] != -1)
						close(temp->pipe_fd[1]);
				}
				temp = temp->next;
			}

			// Set up input from previous command
			if (prev)
			{
				if (prev->pipe_fd[0] != -1)
				{
					dup2(prev->pipe_fd[0], STDIN_FILENO);
					close(prev->pipe_fd[0]);
				}
				if (prev->pipe_fd[1] != -1)
					close(prev->pipe_fd[1]);
			}

			// Set up output to next command
			if (current->next)
			{
				if (current->pipe_fd[1] != -1)
				{
					dup2(current->pipe_fd[1], STDOUT_FILENO);
					close(current->pipe_fd[1]);
				}
				if (current->pipe_fd[0] != -1)
					close(current->pipe_fd[0]);
			}

			if (!setup_redirections(current->redirects))
				exit(1);

			if (current->args && current->args[0])
			{
				if (is_builtin(current->args[0]))
					exit(exec_builtin(current, env_list, shell));
				else
					exit(execute_external_cmd(current, *env_list));
			}
			exit(0);
		}
		else if (pids[i] < 0)
		{
			perror("fork");
			goto cleanup;
		}
		prev = current;
		current = current->next;
		i++;
	}

	// Close all pipes in parent process
	current = cmd_list;
	while (current)
	{
		if (current->pipe_fd[0] != -1)
		{
			close(current->pipe_fd[0]);
			current->pipe_fd[0] = -1;
		}
		if (current->pipe_fd[1] != -1)
		{
			close(current->pipe_fd[1]);
			current->pipe_fd[1] = -1;
		}
		current = current->next;
	}

	// Wait for all child processes
	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &exit_status, 0);
		if (WIFEXITED(exit_status))
			exit_status = WEXITSTATUS(exit_status);
		i++;
	}

	// Restore stdin/stdout
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);

cleanup:
	close(stdin_backup);
	close(stdout_backup);
	free(pids);
	return (exit_status);
}

/* 执行器主函数 */
int	executor(t_cmd *cmd_list, t_env **env_list)
{
	if (!cmd_list)
		return (0);

	// 创建一个临时 shell 结构或从全局获取
	t_shell shell;
	shell.env_list = *env_list;
	shell.cmd_list = cmd_list;
	shell.exit_status = 0;

	return (execute_commands(cmd_list, env_list, &shell));
}
