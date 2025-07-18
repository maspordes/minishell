/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/17 19:01:39 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_input_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_error("minishell", filename, strerror(errno));
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

int	handle_output_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_error("minishell", filename, strerror(errno));
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	handle_append_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		print_error("minishell", filename, strerror(errno));
		return (0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	handle_heredoc_redirect(char *delimiter)
{
	int	fd;

	fd = handle_heredoc(delimiter);
	if (fd == -1)
		return (0);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}
