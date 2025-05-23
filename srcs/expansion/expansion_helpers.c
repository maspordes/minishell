/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:52:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 23:53:36 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
