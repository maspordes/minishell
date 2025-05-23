/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 22:04:02 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_redirect(t_cmd *cmd, t_redirect *redirect)
{
	t_redirect	*current;

	if (!cmd->redirects)
	{
		cmd->redirects = redirect;
		return ;
	}
	current = cmd->redirects;
	while (current->next)
		current = current->next;
	current->next = redirect;
}

int	handle_redirect(t_token **token, t_cmd *cmd)
{
	int			type;
	t_token		*file_token;

	type = (*token)->type;
	*token = (*token)->next;
	if (!*token || (*token)->type != T_WORD)
		return (0);
	file_token = *token;
	add_redirect(cmd, new_redirect(type, file_token->value));
	*token = (*token)->next;
	return (1);
}
