/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:01:53 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 19:01:53 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variables(char *str, t_env *env_list, int exit_status)
{
	char	*expanded;
	char	*key;
	char	*value;
	int		i;

	expanded = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			i++;
			key = get_key_from_str(&str[i]);
			if (ft_strcmp(key, "?") == 0)
				value = ft_itoa(exit_status);
			else
				value = get_env_value(env_list, key);
			expanded = ft_strjoin_free(expanded, value ? value : "");
			free(key);
			if (value && ft_strcmp(key, "?") != 0)
				free(value);
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
		}
		else
			expanded = ft_strjoin_char(expanded, str[i++]);
	}
	return (expanded);
}

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

int	is_numeric_argument(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (str[0] == '+' || str[0] == '-')
		str++;
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
