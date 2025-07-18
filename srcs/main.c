/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/17 23:38:35 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_signal_status = 0;

static void	handle_input_loop(t_shell *shell)
{
	char	*current_input;

	while (1)
	{
		// handle_sigint_prompt();
		current_input = read_input();
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
			}
		}
		else
		{
			free(current_input);
		}
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
	printf("%i\n", g_signal_status);
	if (g_signal_status == 130) 
		shell->exit_status = 130;
	else
		last_status = shell->exit_status;
	restore_terminal_state();
	free_shell(shell);
	return (last_status);
}
