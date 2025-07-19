/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 22:01:17 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_special_char(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

static char	*process_quoted_segment(char *input, int *i)
{
	char	quote_char;
	int		start;

	quote_char = input[*i];
	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i] == quote_char)
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

static char	*process_unquoted_segment(char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i] && !is_special_char(input[*i])
		&& input[*i] != '\'' && input[*i] != '\"')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

static char	*join_and_free(char *s1, char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

void	handle_word(char *input, int *i, t_token **tokens)
{
	char	*final_word_value;
	char	*segment;

	final_word_value = ft_strdup("");
	if (!final_word_value)
		return ;
	while (input[*i] && !is_special_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '\"')
			segment = process_quoted_segment(input, i);
		else
			segment = process_unquoted_segment(input, i);
		if (segment)
		{
			final_word_value = join_and_free(final_word_value, segment);
			if (!final_word_value)
				return ;
		}
		else
			break ;
	}
	if (ft_strlen(final_word_value) > 0)
		add_token(tokens, new_token(final_word_value, T_WORD));
	else
		free(final_word_value);
}
