/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 23:53:36 by shutan           ###   ########.fr       */
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

char	*process_variable(char *str, int *i, t_env *env_list,
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
	char			*expanded;
	t_expand_data	data;
	t_expand_state	state;

	data.env_list = env_list;
	data.exit_status = exit_status;
	state.i = 0;
	state.in_single_quote = 0;
	state.in_double_quote = 0;
	expanded = ft_strdup("");
	return (process_expansion_loop(str, expanded, &data, &state));
}
