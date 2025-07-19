/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_special.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 22:05:27 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_pipe_token(char *input, int *i, t_token **tokens)
{
	(void)input;
	add_token(tokens, new_token(ft_strdup("|"), T_PIPE));
	(*i)++;
	return (1);
}

static int	handle_input_redir(char *input, int *i, t_token **tokens)
{
	if (input[*i + 1] == '<')
	{
		add_token(tokens, new_token(ft_strdup("<<"), T_HEREDOC));
		(*i) += 2;
	}
	else
	{
		add_token(tokens, new_token(ft_strdup("<"), T_REDIR_IN));
		(*i)++;
	}
	return (1);
}

static int	handle_output_redir(char *input, int *i, t_token **tokens)
{
	if (input[*i + 1] == '>')
	{
		add_token(tokens, new_token(ft_strdup(">>"), T_REDIR_APPEND));
		(*i) += 2;
	}
	else
	{
		add_token(tokens, new_token(ft_strdup(">"), T_REDIR_OUT));
		(*i)++;
	}
	return (1);
}

int	handle_special_char(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '|')
		return (handle_pipe_token(input, i, tokens));
	else if (input[*i] == '<')
		return (handle_input_redir(input, i, tokens));
	else if (input[*i] == '>')
		return (handle_output_redir(input, i, tokens));
	return (0);
}
