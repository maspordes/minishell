/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:30:00 by marrey            #+#    #+#             */
/*   Updated: 2025/05/12 22:18:02 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Function: remove_quotes_from_str
 * --------------------------------
 * Creates a new string with syntactical quote characters (' and ") removed.
 * This should be called *after* variable expansion.
 * Preserves quote characters that are themselves quoted (e.g., ' in "it's"
 * or " in 'say "hi"').
 *
 * str: The input string (potentially after variable expansion).
 * returns: A new string with quotes removed. NULL on allocation error.
 */
char	*remove_quotes_from_str(char *str)
{
	char			*result;
	int				i; /* Index for input string */
	int				j; /* Index for result string */
	char			in_single_quote;
	char			in_double_quote;

	if (!str)
		return (NULL);
	result = (char *)malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++; /* Skip the quote character itself */
		}
		else if (str[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++; /* Skip the quote character itself */
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
} 