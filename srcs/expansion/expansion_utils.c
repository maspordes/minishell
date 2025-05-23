/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 23:10:23 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_variable_value(char *key, t_env *env_list, int exit_status)
{
	char	*value;
	char	*env_value;

	if (ft_strcmp(key, "?") == 0)
		value = ft_itoa(exit_status);
	else
	{
		env_value = get_env_value(env_list, key);
		if (env_value)
			value = ft_strdup(env_value);
		else
			value = NULL;
	}
	return (value);
}

static int	skip_variable_name(char *str, int i)
{
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

static char	*process_variable(char *str, int *i, t_env *env_list,
	int exit_status)
{
	char	*key;
	char	*value;
	int		start;

	(*i)++;
	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		key = ft_strdup("?");
	}
	else if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		*i = skip_variable_name(str, *i);
		key = ft_substr(str, start, *i - start);
	}
	else
	{
		(*i)--;
		return (ft_strdup("$"));
	}
	value = get_variable_value(key, env_list, exit_status);
	free(key);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

char	*expand_variables(char *str, t_env *env_list, int exit_status)
{
	char	*expanded;
	char	*value;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	expanded = ft_strdup("");
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '$' && str[i + 1] && !in_single_quote)
		{
			value = process_variable(str, &i, env_list, exit_status);
			if (value)
			{
				expanded = ft_strjoin_free(expanded, value);
				free(value);
			}
			continue;
		}
		expanded = ft_strjoin_char(expanded, str[i++]);
	}
	return (expanded);
}
