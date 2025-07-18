/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:30:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/17 19:43:18 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static volatile pid_t	g_readline_pid = 0;
static volatile int		g_heredoc_interrupted = 0;

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	if (g_readline_pid > 0)
	{
		kill(g_readline_pid, SIGINT);
		g_readline_pid = 0;
	}
	write(STDOUT_FILENO, "\n", 1);
}

static int	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (-1);
	signal(SIGQUIT, SIG_IGN);
	return (0);
}

static void	restore_heredoc_signals(void)
{
	setup_signals();
	if (g_heredoc_interrupted)
		setup_readline();
}

static int	child_readline_process(int pipe_fd[2])
{
	char	*line;

	close(pipe_fd[0]);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	line = readline("> ");
	if (!line)
	{
		close(pipe_fd[1]);
		exit(1);
	}
	write(pipe_fd[1], line, ft_strlen(line));
	write(pipe_fd[1], "\n", 1);
	free(line);
	close(pipe_fd[1]);
	exit(0);
}

static char	*read_heredoc_line(void)
{
	int		pipe_fd[2];
	pid_t	pid;
	char	buffer[1024];
	ssize_t	bytes_read;
	int		status;
	char	*line;
	int		i;

	if (g_heredoc_interrupted)
		return (NULL);
	if (pipe(pipe_fd) == -1)
		return (NULL);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (NULL);
	}
	if (pid == 0)
		child_readline_process(pipe_fd);
	g_readline_pid = pid;
	close(pipe_fd[1]);
	bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	close(pipe_fd[0]);
	waitpid(pid, &status, 0);
	g_readline_pid = 0;
	if (g_heredoc_interrupted)
	{
		setup_readline();
		return (NULL);
	}
	if (bytes_read <= 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	i = bytes_read - 1;
	while (i >= 0 && (buffer[i] == '\n' || buffer[i] == '\r'))
		buffer[i--] = '\0';
	line = ft_strdup(buffer);
	return (line);
}

int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	g_heredoc_interrupted = 0;
	g_readline_pid = 0;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	if (setup_heredoc_signals() == -1)
		return (-1);
	while (1)
	{
		line = read_heredoc_line();
		if (!line)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	restore_heredoc_signals();
	close(pipe_fd[1]);
	if (g_heredoc_interrupted)
	{
		close(pipe_fd[0]);
		g_signal_status = 130;
		return (-1);
	}
	return (pipe_fd[0]);
}
