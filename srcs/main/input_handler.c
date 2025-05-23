/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 22:50:09 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <termios.h>
#include <unistd.h>

void	setup_readline(void)
{
	struct termios	term;

	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &term) == -1)
			return ;
		term.c_lflag &= ~ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		{
			return ;
		}
	}
}

void	handle_sigint_prompt(void)
{
	if (g_signal_status == 130)
	{
		g_signal_status = 0;
		printf("\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

char	*read_input(void)
{
	char	*prompt;

	prompt = readline("minishell> ");
	if (!prompt)
	{
		printf("exit\n");
		return (NULL);
	}
	return (prompt);
}
