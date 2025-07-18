/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:07:21 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 18:56:27 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_status = 0;

static void	handle_input_loop(t_shell *shell)
{
	char	*current_input;
	char	buffer[4096];
	ssize_t	bytes_read;
	char	*input_copy;
	char	*line;
	char	*newline_pos;
	char	*exit_str;

	if (!isatty(STDIN_FILENO))
	{
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			input_copy = ft_strdup(buffer);
			if (!input_copy)
				return ;
			line = input_copy;
			while (line && *line)
			{
				newline_pos = ft_strchr(line, '\n');
				if (newline_pos)
				{
					*newline_pos = '\0';
					newline_pos++;
				}
				if (ft_strlen(line) > 0)
				{
					shell->input = ft_strdup(line);
					if (shell->input)
					{
						process_input(shell);
						clean_current_command(shell);
						if (shell->should_exit)
							break ;
					}
				}
				if (!newline_pos)
					break ;
				line = newline_pos;
			}
			free(input_copy);
		}
		return ;
	}
	while (1)
	{
		current_input = read_input();
		if (g_signal_status == 130)
		{
			shell->exit_status = 130;
			exit_str = ft_itoa(130);
			if (exit_str)
			{
				set_env_value(&(shell->env_list), "?", exit_str);
				free(exit_str);
			}
			g_signal_status = 0;
		}
		if (!current_input)
			break ;
		if (current_input[0] != '\0')
		{
			shell->input = ft_strdup(current_input);
			free(current_input);
			if (shell->input)
			{
				add_history(shell->input);
				process_input(shell);
				clean_current_command(shell);
				if (shell->should_exit)
					break ;
			}
		}
		else
			free(current_input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		last_status;

	(void)argc;
	(void)argv;
	shell = init_shell(envp);
	if (!shell)
		return (1);
	setup_signals();
	setup_readline();
	handle_input_loop(shell);
	last_status = shell->exit_status;
	rl_clear_history();
	rl_cleanup_after_signal();
	restore_terminal_state();
	free_shell(shell);
	return (last_status);
}
