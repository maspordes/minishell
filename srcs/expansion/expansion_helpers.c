/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:52:00 by shutan            #+#    #+#             */
/*   Updated: 2025/06/18 18:04:59 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_special_vars(char *str, int *i)
{
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	else if (str[*i] == '$')
	{
		(*i)++;
		return (ft_strdup("$"));
	}
	return (NULL);
}

char	*handle_normal_var(char *str, int *i, int start)
{
	*i = skip_variable_name(str, *i);
	return (ft_substr(str, start, *i - start));
}

char	*get_variable_value(char *key, t_env *env_list, int exit_status)
{
	char	*value;
	char	*env_value;

	if (ft_strcmp(key, "?") == 0)
		value = ft_itoa(exit_status);
	else if (ft_strcmp(key, "$") == 0)
		value = ft_itoa(getpid());
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

int	skip_variable_name(char *str, int i)
{
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*handle_variable_expansion(char *str, int *i, char *expanded,
	t_expand_data *data)
{
	char	*value;

	value = process_variable(str, i, data->env_list, data->exit_status);
	if (value)
	{
		expanded = ft_strjoin_free(expanded, value);
		free(value);
	}
	return (expanded);
}

char	*process_expansion_loop(char *str, char *expanded,
	t_expand_data *data, t_expand_state *state)
{
	while (str[state->i])
	{
		if (str[state->i] == '\'' && !state->in_double_quote)
			state->in_single_quote = !state->in_single_quote;
		else if (str[state->i] == '"' && !state->in_single_quote)
			state->in_double_quote = !state->in_double_quote;
		else if (str[state->i] == '$' && str[state->i + 1]
			&& !state->in_single_quote)
		{
			expanded = handle_variable_expansion(str, &state->i, expanded,
					data);
			continue ;
		}
		expanded = ft_strjoin_char(expanded, str[state->i++]);
	}
	return (expanded);
}
