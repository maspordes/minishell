/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 22:04:11 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_redirect_token(t_token_type type)
{
	return (type >= T_REDIR_IN && type <= T_HEREDOC);
}

static t_cmd	*process_token(t_token **token, t_cmd *current)
{
	if ((*token)->type == T_WORD)
	{
		if (!add_arg(current, (*token)->value))
			return (NULL);
		*token = (*token)->next;
	}
	else if ((*token)->type == T_PIPE)
	{
		current = handle_pipe(token, current);
		if (!current)
			return (NULL);
	}
	else if (is_redirect_token((*token)->type))
	{
		if (!handle_redirect(token, current))
			return (NULL);
	}
	else
		*token = (*token)->next;
	return (current);
}

static t_cmd	*parse_cmd(t_token **token)
{
	t_cmd	*cmd;
	t_cmd	*current;

	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	current = cmd;
	while (*token)
	{
		current = process_token(token, current);
		if (!current)
			return (NULL);
	}
	return (cmd);
}

t_cmd	*parser(t_token *tokens)
{
	t_token	*token_ptr;
	t_cmd	*cmd_list;

	token_ptr = tokens;
	cmd_list = parse_cmd(&token_ptr);
	return (cmd_list);
}
