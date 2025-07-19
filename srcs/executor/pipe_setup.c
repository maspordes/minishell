/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 21:53:49 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	close_unused_pipes(t_cmd *current, t_cmd *prev, t_cmd *cmd_list)
{
	t_cmd	*iter;

	iter = cmd_list;
	while (iter)
	{
		if (iter != current && (!prev || iter != prev))
		{
			if (iter->pipe_fd[0] != -1)
				close(iter->pipe_fd[0]);
			if (iter->pipe_fd[1] != -1)
				close(iter->pipe_fd[1]);
		}
		iter = iter->next;
	}
}

static void	setup_input_pipe(t_cmd *prev)
{
	if (prev && prev->pipe_fd[0] != -1)
	{
		dup2(prev->pipe_fd[0], STDIN_FILENO);
		close(prev->pipe_fd[0]);
		if (prev->pipe_fd[1] != -1)
			close(prev->pipe_fd[1]);
	}
}

static void	setup_output_pipe(t_cmd *current)
{
	if (current->next && current->pipe_fd[1] != -1)
	{
		dup2(current->pipe_fd[1], STDOUT_FILENO);
		close(current->pipe_fd[1]);
		if (current->pipe_fd[0] != -1)
			close(current->pipe_fd[0]);
	}
}

void	setup_child_pipes(t_cmd *current, t_cmd *prev, t_cmd *cmd_list)
{
	close_unused_pipes(current, prev, cmd_list);
	setup_input_pipe(prev);
	setup_output_pipe(current);
}
