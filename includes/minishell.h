/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 19:24:50 by shutan            #+#    #+#             */
/*   Updated: 2025/06/18 18:03:17 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <dirent.h>
# include <termios.h>

/* 信号处理的全局变量 */
extern int	g_signal_status;

/* 标记类型枚举 */
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_HEREDOC,
	T_AND,
	T_OR,
	T_LPAREN,
	T_RPAREN
}	t_token_type;

/* 标记结构 */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/* 命令重定向结构 */
typedef struct s_redirect
{
	int					type;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

/* 命令结构 */
typedef struct s_cmd
{
	char			**args;
	t_redirect		*redirects;
	struct s_cmd	*next;
	int				pipe_fd[2];
}	t_cmd;

/* 环境变量结构 */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* 主要shell结构 */
typedef struct s_shell
{
	t_env	*env_list;
	t_cmd	*cmd_list;
	t_token	*tokens;
	char	*input;
	int		exit_status;
}	t_shell;

/* 执行器数据结构 */
typedef struct s_exec_data
{
	t_cmd	*cmd_list;
	t_env	**env_list;
	t_shell	*shell;
}	t_exec_data;

/* 扩展数据结构 */
typedef struct s_expand_data
{
	t_env	*env_list;
	int		exit_status;
}	t_expand_data;

/* 扩展状态结构 */
typedef struct s_expand_state
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;
}	t_expand_state;

/* 函数原型 */

/* 词法分析器 */
t_token	*lexer(char *input);
void	free_tokens(t_token *tokens);
t_token	*new_token(char *value, t_token_type type);
void	add_token(t_token **tokens, t_token *new_token);
int		handle_special_char(char *input, int *i, t_token **tokens);
void	handle_word(char *input, int *i, t_token **tokens);

/* 语法分析器 */
t_cmd	*parser(t_token *tokens);
void	free_cmds(t_cmd *cmds);
t_cmd	*new_cmd(void);
t_redirect	*new_redirect(int type, char *file);
void	add_redirect(t_cmd *cmd, t_redirect *redirect);
int		handle_redirect(t_token **token, t_cmd *cmd);
int		add_arg(t_cmd *cmd, char *arg);
t_cmd	*handle_pipe(t_token **token, t_cmd *cmd);

/* 执行器 */
int		executor(t_cmd *cmd_list, t_env **env_list, t_shell *shell);
char	*find_executable(char *cmd, t_env *env_list);
void	free_array(char **array);
int		handle_heredoc(char *delimiter);
int		setup_redirections(t_redirect *redirects);
int		is_parent_builtin(const char *cmd_name);
int		is_single_parent_builtin(t_cmd *cmd_list, int num_cmds);
int		execute_builtin_command(t_cmd *cmd_list, t_env **env_list,
			t_shell *shell);
int		handle_input_redirect(char *filename);
int		handle_output_redirect(char *filename);
int		handle_append_redirect(char *filename);
int		handle_heredoc_redirect(char *delimiter);

/* 内置命令 */
int		ft_echo(char **args, t_env *env_list);
int		ft_cd(char **args, t_env **env_list);
int		ft_pwd(void);
int		ft_export(char **args, t_env **env_list);
int		ft_unset(char **args, t_env **env_list);
int		ft_env(t_env *env_list);
int		ft_exit(char **args, t_shell *shell);

/* 环境变量 */
t_env	*init_env(char **envp);
char	*get_env_value(t_env *env_list, const char *key);
void	set_env_value(t_env **env_list, char *key, char *value);
void	free_env(t_env *env_list);
char	**env_to_array(t_env *env_list);
void	add_or_update_env(t_env **env_list, const char *arg);

/* 信号处理 */
void	setup_signals(void);
void	reset_signals(void);

/* 工具函数 */
int		is_builtin(char *cmd);
int		exec_builtin(t_cmd *cmd, t_env **env_list, t_shell *shell);
char	*expand_variables(char *str, t_env *env_list, int exit_status);
char	*ft_strjoin_char(char *s1, char c);
char	*ft_strjoin_free(char *s1, const char *s2);
void	print_error(const char *prefix, const char *arg, const char *message);

/* Filename Utils */
char	*process_filename_quotes(const char *raw_filename);

/* Main module functions */
t_shell	*init_shell(char **envp);
void	clean_current_command(t_shell *shell);
void	free_shell(t_shell *shell);
void	setup_readline(void);
void	handle_sigint_prompt(void);
char	*read_input(void);
int		execute_pipeline(t_shell *shell);
int		process_input(t_shell *shell);

/* Other utility functions */
void	print_export_env(t_env *env_list);
char	*get_key_from_str(const char *str);

/* readline 函数声明 - 条件编译 */
/* Note: readline functions are declared in readline/readline.h */

/* Expansion module */
int		expand_command(t_cmd *cmd, t_shell *shell);
char	*expand_variables_in_str(char *str, t_shell *shell);
char	*remove_quotes_from_str(char *str);
char	*process_variable(char *str, int *i, t_env *env_list, int exit_status);
char	*handle_variable_expansion(char *str, int *i, char *expanded,
			t_expand_data *data);
char	*process_expansion_loop(char *str, char *expanded,
			t_expand_data *data, t_expand_state *state);
char	**create_empty_result(void);
char	**create_single_result(char *str);
void	free_str_array(char **array);
void	cleanup_args_on_error(char **new_args, int j);
char	**perform_single_expansion(char *token, t_shell *shell);
int		process_single_arg(char **new_args, int *j, char *arg, t_shell *shell);

/* Parser module */
t_cmd	*parser(t_token *tokens);
void	free_cmds(t_cmd *cmd_list);

// expansion_helpers.c
char	*handle_special_vars(char *str, int *i);
char	*handle_normal_var(char *str, int *i, int start);
char	*get_variable_value(char *key, t_env *env_list, int exit_status);
int		skip_variable_name(char *str, int i);

#endif
