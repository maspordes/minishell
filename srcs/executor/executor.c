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

/* 关闭所有管道 */
static void	close_pipes(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (current->pipe_fd[0] != -1)
			close(current->pipe_fd[0]);
		if (current->pipe_fd[1] != -1)
			close(current->pipe_fd[1]);
		current = current->next;
	}
}

/* 设置管道重定向 */
static void	setup_pipes(t_cmd *cmd, t_cmd *prev)
{
	if (prev && prev->pipe_fd[0] != -1)
	{
		dup2(prev->pipe_fd[0], STDIN_FILENO);
		close(prev->pipe_fd[0]);
	}
	if (cmd->next && cmd->pipe_fd[1] != -1)
	{
		dup2(cmd->pipe_fd[1], STDOUT_FILENO);
		close(cmd->pipe_fd[1]);
	}
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
	int		exit_status;
	int		stdin_backup;
	int		stdout_backup;
	char	*exit_str;

	if (!create_pipes(cmd_list))
		return (1);

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

	current = cmd_list;
	prev = NULL;
	exit_status = 0;

	while (current)
	{
		setup_pipes(current, prev);

		if (!setup_redirections(current->redirects))
		{
			exit_status = 1;
			goto cleanup;
		}

		// 执行命令
		if (current->args && current->args[0])
		{
			if (is_builtin(current->args[0]))
				exit_status = exec_builtin(current, env_list, shell);
			else
				exit_status = execute_external_cmd(current, *env_list);
		}

		// 设置 $? 环境变量
		exit_str = ft_itoa(exit_status);
		set_env_value(env_list, "?", exit_str);
		free(exit_str);

		// 恢复标准输入输出
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);

		prev = current;
		current = current->next;
	}

cleanup:
	close_pipes(cmd_list);
	close(stdin_backup);
	close(stdout_backup);
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
