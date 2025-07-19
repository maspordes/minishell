/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:07:30 by shutan            #+#    #+#             */
/*   Updated: 2025/07/19 17:50:16 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <termios.h>

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
