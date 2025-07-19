/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 22:59:32 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*prepare_filename(t_redirect *redirect)
{
	char	*processed_filename;

	if (redirect->type != T_HEREDOC)
		processed_filename = process_filename_quotes(redirect->file);
	else
		processed_filename = ft_strdup(redirect->file);
	if (!processed_filename)
	{
		ft_putstr_fd("minishell: memory allocation error\n", 2);
		return (NULL);
	}
	if (processed_filename[0] == '\0')
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		free(processed_filename);
		return (NULL);
	}
	return (processed_filename);
}

static int	execute_redirect(t_redirect *redirect, char *filename)
{
	if (redirect->type == T_REDIR_IN)
		return (handle_input_redirect(filename));
	else if (redirect->type == T_REDIR_OUT)
		return (handle_output_redirect(filename));
	else if (redirect->type == T_REDIR_APPEND)
		return (handle_append_redirect(filename));
	else if (redirect->type == T_HEREDOC)
		return (handle_heredoc_redirect(filename));
	return (0);
}

static int	process_single_redirect(t_redirect *redirect)
{
	char	*processed_filename;
	int		result;

	processed_filename = prepare_filename(redirect);
	if (!processed_filename)
		return (0);
	result = execute_redirect(redirect, processed_filename);
	free(processed_filename);
	return (result);
}

int	setup_redirections(t_redirect *redirects)
{
	while (redirects)
	{
		if (!process_single_redirect(redirects))
			return (0);
		redirects = redirects->next;
	}
	return (1);
}
