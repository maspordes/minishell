/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/05/12 20:46:18 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <termios.h>

int	g_signal_status = 0;

/* 初始化shell结构 */
static t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_list = init_env(envp);
	shell->cmd_list = NULL;
	shell->tokens = NULL;
	shell->input = NULL;
	shell->exit_status = 0;
	return (shell);
}

/* 清理当前命令的资源 */
static void	clean_current_command(t_shell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->cmd_list)
	{
		free_cmds(shell->cmd_list);
		shell->cmd_list = NULL;
	}
}

/* 释放shell结构 */
void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	clean_current_command(shell);
	if (shell->env_list)
		free_env(shell->env_list);
	free(shell);
}

/* 处理输入行 */
static int	process_input(t_shell *shell)
{
	if (!shell->input || shell->input[0] == '\0')
		return (0);
	add_history(shell->input);
	shell->tokens = lexer(shell->input);
	if (!shell->tokens)
		return (0);
	shell->cmd_list = parser(shell->tokens);
	if (!shell->cmd_list)
		return (0);
	
	// 执行命令并保存退出状态码
	shell->exit_status = executor(shell->cmd_list, &(shell->env_list));
	return (shell->exit_status);
}

/* 设置终端属性 */
static void setup_readline(void)
{
	struct termios term;

	// 获取当前终端设置
	tcgetattr(STDIN_FILENO, &term);

	// 配置终端以更接近 Bash 的行为
	// ECHOCTL: 控制是否回显控制字符（如 ^C）
	term.c_lflag &= ~ECHOCTL;  // 禁用控制字符回显

	// 应用新的终端设置
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	char	*prompt;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (1);
	setup_signals();
	setup_readline();

	while (1)
	{
		// 检查是否有信号中断
		if (g_signal_status == 130) /* Check specific signal status if needed */
		{
			// 重置信号状态
			g_signal_status = 0;
			// Maybe update $? to 130?
			printf("\n"); /* Print newline after ^C */
			rl_on_new_line(); /* Tell readline we are on a new line */
			rl_replace_line("", 0); /* Clear current buffer */
			/* Do not redisplay here, readline will handle it */
			continue; /* Go straight to readline */
		}
		
		// 使用 readline 的内置提示符
		prompt = readline("minishell> ");
		
		if (!prompt)  /* Ctrl+D or EOF */
		{
			printf("exit\n");
			break; /* Exit loop */
		}
		
		if (prompt[0] != '\0')
		{
			shell->input = ft_strdup(prompt);
			/* process_input now handles add_history */
			process_input(shell);
			free(prompt);
			clean_current_command(shell);
		}
		else /* Empty line entered */
		{
			free(prompt);
		}
	}
	free_shell(shell);
	return (shell->exit_status); /* Return last command's status */
} 