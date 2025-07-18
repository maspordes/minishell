/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/07/17 21:29:10 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <termios.h>
#include <unistd.h>

static struct termios	g_original_term;
static int				g_term_saved = 0;

static int	save_terminal_state(void)
{
	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &g_original_term) == -1)
			return (-1);
		g_term_saved = 1;
	}
	return (0);
}

static int	setup_terminal_for_readline(void)
{
	struct termios	term;

	if (!isatty(STDIN_FILENO))
		return (0);
	if (!g_term_saved)
	{
		if (save_terminal_state() == -1)
			return (-1);
	}
	term = g_original_term;
	term.c_lflag &= ~ECHOCTL;
	term.c_cc[VQUIT] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return (-1);
	return (0);
}

void	setup_readline(void)
{
	if (setup_terminal_for_readline() == -1)
		return ;
}

void	restore_terminal_state(void)
{
	if (g_term_saved && isatty(STDIN_FILENO))
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &g_original_term);
	}
}

void	handle_sigint_prompt(void)
{
	if (g_signal_status == 130)
	{
		//g_signal_status = 0;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

static char	*get_prompt_string(void)
{
	if (isatty(STDIN_FILENO))
		return ("minishell> ");
	return ("");
}

char	*read_input(void)
{
	char	*prompt_str;
	char	*input;

	prompt_str = get_prompt_string();
	input = readline(prompt_str);
	if (!input)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "exit\n", 5);
		restore_terminal_state();
		return (NULL);
	}
	return (input);
}
