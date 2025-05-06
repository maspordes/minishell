/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:30:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 21:18:44 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** 处理 heredoc 重定向
** 读取输入直到遇到指定的分隔符
** 返回一个文件描述符，其中包含读取的内容
*/
int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	
	// 禁用信号处理（避免干扰heredoc读取）
	signal(SIGINT, SIG_DFL);
	
	while (1)
	{
		// 使用 readline 读取输入（不保存到历史记录）
		line = readline("> ");
		
		// 如果读取失败或遇到 EOF，退出循环
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break;
		}
		
		// 写入读取的行到管道
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	
	// 恢复信号处理
	setup_signals();
	
	// 关闭写入端，返回读取端
	close(pipe_fd[1]);
	return (pipe_fd[0]);
} 