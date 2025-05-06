/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/06 17:14:11 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../../includes/minishell.h"

/* 处理 SIGINT 信号（Ctrl+C）*/
static void	handle_sigint(int sig)
{
	(void)sig;
	
	// 设置信号状态标记
	g_signal_status = 130;
	
	// 只写入一个换行符
	write(STDOUT_FILENO, "\n", 1);
	
	// 告诉 readline 重新显示
	rl_on_new_line();
	
#ifdef __linux__
	// Linux 平台特定代码 - 使用完整的 GNU readline 功能
	rl_replace_line("", 0);
	rl_redisplay();
#endif
	
	// 重新设置信号处理函数
	signal(SIGINT, handle_sigint);
}

/* 处理 SIGQUIT 信号（Ctrl+\）*/
static void	handle_sigquit(int sig)
{
	(void)sig;
	/* 不做任何事情 */
}

/* 设置交互模式的信号处理 */
void	setup_signals(void)
{
	// 使用更简单的 signal() 函数
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

/* 重置信号处理为默认行为 */
void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
} 