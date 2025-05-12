/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:43:14 by marrey            #+#    #+#             */
/*   Updated: 2025/05/12 20:57:14 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
 * Strips a single layer of surrounding quotes (' or ") from a string.
 * Returns a newly allocated string, or a strdup if no surrounding quotes.
 * Returns NULL on allocation failure.
 */
char * strip_surrounding_quotes(const char *str)
{
    int len;

    if (!str)
        return (NULL);
    len = ft_strlen(str);
    if (len >= 2)
    {
        if ((str[0] == '\'' && str[len - 1] == '\'') || \
            (str[0] == '"' && str[len - 1] == '"'))
        {
            return (ft_substr(str, 1, len - 2));
        }
    }
    return (ft_strdup(str));
}

/*
 * Placeholder for more complex filename processing (like handling "a"b'c').
 * For now, just uses strip_surrounding_quotes.
 * Returns a newly allocated processed filename.
 */
char * process_filename_quotes(const char *raw_filename)
{
    // Basic implementation for now
    // TODO: Implement proper handling for concatenations like "file"\' name\' etc.
    return (strip_surrounding_quotes(raw_filename));
}

/*
** Performs quote removal and concatenation for a filename.
** E.g., "file"'name' -> filename
**        "a""b"c -> abc
*/
char	*expand_filename_quotes(const char *raw_filename)
{
	char	*expanded_name;
	char	*buffer;
	int		i;
	int		j;
	char	current_quote;

	if (!raw_filename)
		return (NULL);
	buffer = (char *)malloc(ft_strlen(raw_filename) + 1);
	if (!buffer)
		return (NULL);
	i = 0;
	j = 0;
	current_quote = 0;
	while (raw_filename[i])
	{
		if (current_quote)
		{
			if (raw_filename[i] == current_quote)
				current_quote = 0; /* End of quoted section */
			else
				buffer[j++] = raw_filename[i];
		}
		else
		{
			if (raw_filename[i] == '\'' || raw_filename[i] == '\"')
				current_quote = raw_filename[i]; /* Start of quoted section */
			else
				buffer[j++] = raw_filename[i];
		}
		i++;
	}
	buffer[j] = '\0';
	/* Handle potential unclosed quotes if necessary */
	if (current_quote)
	{
		/* Behavior for unclosed quotes is undefined here, parser should catch */
	}
	expanded_name = ft_strdup(buffer);
	free(buffer);
	return (expanded_name);
}