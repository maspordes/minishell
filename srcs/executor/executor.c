/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/12 20:42:11 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Forward declare the new function if needed, or ensure filename_utils.h exists and is included
char	*expand_filename_quotes(const char *raw_filename);

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
	int		fd;
	char	*processed_filename;

	while (redirects)
	{
		/* Use the new expansion function */
		if (redirects->type != T_HEREDOC)
			processed_filename = expand_filename_quotes(redirects->file);
		else
			processed_filename = ft_strdup(redirects->file); /* Heredoc delimiter shouldn't be expanded */

		if (!processed_filename)
		{
			fprintf(stderr, "minishell: memory allocation error\n");
			return (0); /* Indicate failure */
		}
		/* Add check for empty filename after expansion */
		if (processed_filename[0] == '\0')
		{
			fprintf(stderr, "minishell: ambiguous redirect\n"); /* Or some error */
			free(processed_filename);
			return (0);
		}

		if (redirects->type == T_REDIR_IN)
		{
			fd = open(processed_filename, O_RDONLY);
			if (fd == -1)
			{
				fprintf(stderr, "minishell: %s: ", processed_filename);
				perror(NULL); 
				fflush(stderr); /* Ensure error message is printed */
				free(processed_filename);
				return (0); 
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redirects->type == T_REDIR_OUT)
		{
			fd = open(processed_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				fprintf(stderr, "minishell: %s: ", processed_filename);
				perror(NULL);
				fflush(stderr); /* Ensure error message is printed */
				free(processed_filename);
				return (0);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redirects->type == T_REDIR_APPEND)
		{
			fd = open(processed_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				fprintf(stderr, "minishell: %s: ", processed_filename);
				perror(NULL);
				fflush(stderr); /* Ensure error message is printed */
				free(processed_filename);
				return (0);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redirects->type == T_HEREDOC)
		{
			fd = handle_heredoc(processed_filename); /* Use the strduped delimiter */
			if (fd == -1)
			{
				/* handle_heredoc should print its own errors, maybe fflush there too */
				free(processed_filename);
				return (0); 
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		free(processed_filename); /* Free the name processed for this iteration */
		redirects = redirects->next;
	}
	return (1); /* Indicate success */
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
	char	**expanded_args; /* New array for expanded/cleaned args */
	int		i;

	/* Expand/Clean arguments (Quote removal for now) */
	/* Count args */
	i = 0;
	while(cmd->args && cmd->args[i])
		i++;
	expanded_args = (char **)malloc(sizeof(char *) * (i + 1));
	if (!expanded_args)
	{
		perror("malloc for expanded_args");
		return (1); /* Indicate failure */
	}
	/* Expand each arg */
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		/* Use the same quote expansion logic as for filenames */
		expanded_args[i] = expand_filename_quotes(cmd->args[i]);
		if (!expanded_args[i])
		{
			/* Handle allocation failure during expansion */
			perror("expand arg failed");
			free_array(expanded_args); /* Free already expanded args */
			return (1);
		}
		i++;
	}
	expanded_args[i] = NULL;

	/* Find executable using the *first expanded* arg */
	cmd_path = find_executable(expanded_args[0], env_list);
	if (!cmd_path)
	{
		printf("minishell: %s: command not found\n", expanded_args[0]);
		free_array(expanded_args);
		return (127);
	}

	env_array = env_to_array(env_list);

	reset_signals();

	pid = fork();
	if (pid == 0)
	{
		/* Execute with expanded args */
		if (execve(cmd_path, expanded_args, env_array) == -1)
		{
			perror("execve");
			/* Child needs to free allocated memory before exiting */
			free(cmd_path);
			free_array(expanded_args);
			free_array(env_array);
			exit(EXIT_FAILURE);
		}
		/* Should not reach here */
	}
	else if (pid < 0)
	{
		perror("fork");
		free(cmd_path);
		free_array(expanded_args);
		free_array(env_array);
		return (1);
	}

	/* Parent waits */
	waitpid(pid, &status, 0);

	/* Parent cleans up */
	free(cmd_path);
	free_array(expanded_args);
	free_array(env_array);
	setup_signals();

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	/* Handle signaled child? */
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1); /* Default return if not exited or signaled (error?) */
}

/* Helper to count commands in the list */
static int count_commands(t_cmd *cmd_list)
{
	int count = 0;
	t_cmd *current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/* Helper to check if a builtin MUST run in the parent */
static int is_parent_builtin(const char *cmd_name)
{
	if (!cmd_name)
		return (0);
	return (ft_strcmp(cmd_name, "cd") == 0 ||
			ft_strcmp(cmd_name, "export") == 0 ||
			ft_strcmp(cmd_name, "unset") == 0 ||
			ft_strcmp(cmd_name, "exit") == 0);
}

/* Helper to close all pipe fds in a command list */
static void close_all_pipes(t_cmd *cmd_list)
{
	t_cmd *current = cmd_list;
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
}

/* Helper to wait for child processes and get last status */
static int wait_for_children(pid_t *pids, int num_cmds)
{
	int i = 0;
	int status = 0;
	int last_status = 0;

	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (i == num_cmds - 1) // Store status of the last command
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status); // Standard practice for signaled processes
			else
				last_status = 1; // Default error
		}
		i++;
	}
	return (last_status);
}

/* 执行命令链表 */
static int execute_commands(t_cmd *cmd_list, t_env **env_list, t_shell *shell)
{
	/* Variable declarations */
	t_cmd   *current = NULL; /* Initialize current */
	t_cmd   *prev = NULL;   /* Initialize prev */
	int     exit_status;
	int     stdin_backup;
	int     stdout_backup;
	pid_t   *pids;
	int     i;
	int     num_cmds;
	char    *exit_str;
	int     pipeline_error;
	int     redir_status;

	/* Initialization */
	exit_status = 0;
	pids = NULL;
	stdin_backup = -1;
	stdout_backup = -1;
	pipeline_error = 0;
	redir_status = 1; /* Assume success initially */

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
	{
		perror("dup initial fds");
		exit_status = 1;
		pipeline_error = 1;
	}

	if (!pipeline_error)
	{
		num_cmds = count_commands(cmd_list);
		/* Special case: Single command that MUST run in parent */
		if (num_cmds == 1 && cmd_list->args && cmd_list->args[0] &&
			is_parent_builtin(cmd_list->args[0]))
		{
			redir_status = setup_redirections(cmd_list->redirects);
			if (!redir_status)
				exit_status = 1;
			else
				exit_status = exec_builtin(cmd_list, env_list, shell);
			dup2(stdin_backup, STDIN_FILENO); /* Restore FDs */
			dup2(stdout_backup, STDOUT_FILENO);
		}
		else /* Pipelines or commands that can/should run in a child */
		{
			/* Proceed directly to pipeline setup if no prior error */
			if (!pipeline_error)
			{
				 pids = malloc(sizeof(pid_t) * num_cmds);
				 if (!pids || (num_cmds > 1 && !create_pipes(cmd_list)))
				 {
					perror("malloc/pipe setup");
					exit_status = 1;
					pipeline_error = 1;
				 }
				 else
				 { 
					/* Fork loop */
					current = cmd_list; 
					prev = NULL;
					i = 0;
					while (current)
					{
						pids[i] = fork();
						if (pids[i] == 0) /* Child process */
						{
							t_cmd *iter = cmd_list;
							/* Child closes unneeded pipe ends */
							while (iter)
							{
								if (iter != current && (!prev || iter != prev))
								{ 
									if(iter->pipe_fd[0] != -1) close(iter->pipe_fd[0]);
									if(iter->pipe_fd[1] != -1) close(iter->pipe_fd[1]);
								}
								iter = iter->next;
							}
							if (prev && prev->pipe_fd[0] != -1)
							{
								dup2(prev->pipe_fd[0], STDIN_FILENO);
								close(prev->pipe_fd[0]);
								if(prev->pipe_fd[1] != -1) close(prev->pipe_fd[1]);
							}
							if (current->next && current->pipe_fd[1] != -1)
							{
								dup2(current->pipe_fd[1], STDOUT_FILENO);
								close(current->pipe_fd[1]);
								if(current->pipe_fd[0] != -1) close(current->pipe_fd[0]);
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
						else if (pids[i] < 0) /* Fork error */
						{
							perror("fork");
							exit_status = 1;
							pipeline_error = 1;
							break; 
						}
						/* Parent: Close pipes progressively */
						if (prev && prev->pipe_fd[0] != -1) close(prev->pipe_fd[0]);
						if (prev && prev->pipe_fd[1] != -1) close(prev->pipe_fd[1]);
						prev = current;
						current = current->next;
						i++;
					} 
				 }
			}
			close_all_pipes(cmd_list); /* Parent closes all pipes */
			if (!pipeline_error)
				exit_status = wait_for_children(pids, num_cmds);
			/* Restore original FDs */
			dup2(stdin_backup, STDIN_FILENO);
			dup2(stdout_backup, STDOUT_FILENO);
		}
	}
	/* Cleanup */
	if (stdin_backup != -1) close(stdin_backup);
	if (stdout_backup != -1) close(stdout_backup);
	if (pids) free(pids);
	close_all_pipes(cmd_list); /* Final safety close */

	/* Set $? */
	exit_str = ft_itoa(exit_status);
	set_env_value(env_list, "?", exit_str);
	free(exit_str);

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
