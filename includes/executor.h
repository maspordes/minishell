/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 22:47:12 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 23:00:15 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

/* executor_utils.c */
int		is_builtin(char *cmd);
int		exec_builtin(t_cmd *cmd, t_env **env_list, t_shell *shell);
void	free_array(char **array);
int		validate_executor_input(t_cmd *cmd_list, t_shell *shell);
void	update_shell_status(t_shell *shell, int exit_status, t_env **env_list);

/* pipe_utils.c */
int		create_pipes(t_cmd *cmd_list);
void	close_all_pipes(t_cmd *cmd_list);
void	close_prev_pipes(t_cmd *prev);
int		init_pipeline(t_cmd *cmd_list, pid_t **pids, int *num_cmds);
void	cleanup_pipeline(pid_t *pids, int stdin_backup, int stdout_backup);
int		count_commands(t_cmd *cmd_list);

/* redirect_utils.c */
int		handle_redir_in(const char *filename);
int		handle_redir_out(const char *filename);
int		handle_redir_append(const char *filename);
int		handle_heredoc_redir(const char *filename);
char	*process_redirect_filename(t_redirect *redirect);
char	*expand_filename_quotes(const char *filename);

/* redirect_handler.c */
int		setup_redirections(t_redirect *redirects);

/* command_utils.c */
char	*find_executable(char *cmd, t_env *env_list);

/* process_utils.c */
int		wait_for_children(pid_t *pids, int num_cmds);
int		handle_child_process(t_cmd *cmd,
			char *executable_path, char **envp_array);
int		handle_parent_process(pid_t pid,
			char *executable_path, char **envp_array);
char	**env_list_to_array(t_env *env_list);

/* executor.c */
int		execute_commands(t_cmd *cmd_list, t_env **env_list, t_shell *shell);
int		executor(t_cmd *cmd_list, t_env **env_list, t_shell *shell);

#endif 