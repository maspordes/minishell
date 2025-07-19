/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/07/19 22:05:29 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_strjoin_char(char *s1, char c)
{
	char	*result;
	int		len;

	if (!s1)
		return (NULL);
	len = ft_strlen(s1);
	result = malloc(len + 2);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len);
	result[len] = c;
	result[len + 1] = '\0';
	free(s1);
	return (result);
}

int	validate_quotes(const char *input)
{
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	if (!input)
		return (1);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	if (in_single_quote || in_double_quote)
	{
		ft_putstr_fd("minishell: syntax error: unclosed quotes\n", 2);
		return (0);
	}
	return (1);
}
