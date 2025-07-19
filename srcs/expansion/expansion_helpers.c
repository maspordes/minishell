/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:47:13 by shutan            #+#    #+#             */
/*   Updated: 2025/07/19 18:01:02 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*handle_variable_expansion(char *str, int *i, char *expanded,
	t_expand_data *data)
{
	char	*value;
	char	*new_expanded;

	value = process_variable(str, i, data->env_list, data->exit_status);
	if (value)
	{
		new_expanded = ft_strjoin_free(expanded, value);
		free(value);
		if (!new_expanded)
			return (NULL);
		return (new_expanded);
	}
	return (expanded);
}

char	*process_expansion_loop(char *str, char *expanded,
	t_expand_data *data, t_expand_state *state)
{
	char	*new_expanded;

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
			if (!expanded)
				return (NULL);
			continue ;
		}
		new_expanded = ft_strjoin_char(expanded, str[state->i]);
		if (!new_expanded)
		{
			free(expanded);
			return (NULL);
		}
		expanded = new_expanded;
		state->i++;
	}
	return (expanded);
}
