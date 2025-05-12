/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 00:05:00 by marrey            #+#    #+#             */
/*   Updated: 2025/05/12 22:12:18 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Function: print_error
 * ---------------------
 * Prints an error message to STDERR_FILENO in a format similar to bash:
 * prefix: arg: message\n
 *
 * prefix:  Usually the program name (e.g., "minishell").
 * arg:     The argument or context causing the error (optional, can be NULL).
 * message: The error description.
 */
void	print_error(const char *prefix, const char *arg, const char *message)
{
	ft_putstr_fd((char *)prefix, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd((char *)message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
} 