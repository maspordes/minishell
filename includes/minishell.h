/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/05/06 17:14:11 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
# include "../libft/includes/libft.h"
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

/* 函数原型 */

/* 词法分析器 */
t_token	*lexer(char *input);
void	free_tokens(t_token *tokens);

/* 语法分析器 */
t_cmd	*parser(t_token *tokens);
void	free_cmds(t_cmd *cmds);

/* 执行器 */
int		executor(t_cmd *cmd_list, t_env **env_list);

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
char	*get_env_value(t_env *env_list, char *key);
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
void	free_array(char **array);
int		handle_heredoc(char *delimiter);
char	*ft_strjoin_char(char *s1, char c);
char	*ft_strjoin_free(char *s1, const char *s2);

/* 添加到函数原型部分 */
void	free_shell(t_shell *shell);
void	print_export_env(t_env *env_list);
char	*get_key_from_str(const char *str);

/* readline 函数声明 - 条件编译 */
#ifdef __linux__
/* 在 Linux 上声明 readline 函数 */
void	rl_replace_line(const char *text, int clear_undo);
void	rl_redisplay(void);
#endif

#endif
