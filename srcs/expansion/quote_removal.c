/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:37:42 by marrey            #+#    #+#             */
/*   Updated: 2025/05/23 22:48:51 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_quote_state(int *i, int *j, char *in_single, char *in_double)
{
	*i = 0;
	*j = 0;
	*in_single = 0;
	*in_double = 0;
}

static int	handle_quote_char(char c, char *in_single, char *in_double)
{
	if (c == '\'' && !(*in_double))
	{
		*in_single = !(*in_single);
		return (1);
	}
	else if (c == '"' && !(*in_single))
	{
		*in_double = !(*in_double);
		return (1);
	}
	return (0);
}

char	*remove_quotes_from_str(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	in_single_quote;
	char	in_double_quote;

	if (!str)
		return (NULL);
	result = (char *)malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	init_quote_state(&i, &j, &in_single_quote, &in_double_quote);
	while (str[i])
	{
		if (handle_quote_char(str[i], &in_single_quote, &in_double_quote))
			i++;
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
