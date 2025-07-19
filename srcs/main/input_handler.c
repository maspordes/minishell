/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/07/19 17:50:32 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <unistd.h>

void	handle_sigint_prompt(void)
{
	if (g_signal_status == 130)
	{
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

static char	*custom_input_read(const char *prompt)
{
	char	buffer[1024];
	ssize_t	bytes_read;
	int		i;
	char	*result;

	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	i = bytes_read - 1;
	while (i >= 0 && (buffer[i] == '\n' || buffer[i] == '\r'))
		buffer[i--] = '\0';
	result = ft_strdup(buffer);
	return (result);
}

char	*read_input(void)
{
	char	*prompt_str;
	char	*input;
	char	buffer[1024];
	ssize_t	bytes_read;
	int		i;

	if (isatty(STDIN_FILENO))
	{
		prompt_str = get_prompt_string();
		input = custom_input_read(prompt_str);
		if (!input)
			return (NULL);
		return (input);
	}
	else
	{
		bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
		if (bytes_read <= 0)
			return (NULL);
		buffer[bytes_read] = '\0';
		i = bytes_read - 1;
		while (i >= 0 && (buffer[i] == '\n' || buffer[i] == '\r'))
			buffer[i--] = '\0';
		input = ft_strdup(buffer);
		return (input);
	}
}
