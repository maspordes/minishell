/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:07:21 by shutan            #+#    #+#             */
/*   Updated: 2025/07/19 19:51:37 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_status = 0;

static void	handle_input_loop(t_shell *shell)
{
	char	*current_input;
	char	*exit_str;
	char	*trimmed;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			current_input = readline("minishell> ");
		else
		{
			current_input = get_next_line(STDIN_FILENO);
			if (current_input)
			{
				trimmed = ft_strtrim(current_input, "\n");
				free(current_input);
				current_input = trimmed;
			}
		}
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
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (current_input[0] != '\0')
		{
			shell->input = ft_strdup(current_input);
			free(current_input);
			if (shell->input)
			{
				process_input(shell);
				clean_current_command(shell);
				if (shell->should_exit)
					break ;
			}
		}
		else
			free(current_input);
	}
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
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
