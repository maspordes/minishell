/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:46:57 by marrey            #+#    #+#             */
/*   Updated: 2025/05/12 21:55:28 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// // Helper to find and return the value of an environment variable
// // Assumes `env.c` provides a function like this, or we adapt/create it.
// // For now, this is a forward declaration for what we expect.
// char	*get_env_value(t_list *env_list, const char *var_name);

static char	*get_var_value(const char *var_name, t_shell *shell)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(shell->exit_status));
	value = get_env_value(shell->env_list, var_name);
	return (value);
}

// Appends a character to a string, reallocating if necessary.
// Returns the new string, or NULL on failure. Caller must free original if new.
static char	*str_append_char(char *str, char c)
{
	size_t	len;
	char	*new_str;

	len = 0;
	if (str)
		len = ft_strlen(str);
	new_str = (char *)malloc(len + 2);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	if (str)
		ft_strlcpy(new_str, str, len + 1);
	else
		new_str[0] = '\0';
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

// Appends a source string to a destination string, reallocating if necessary.
// Returns the new string, or NULL on failure. Caller must free original dest if new.
static char	*str_append_str(char *dest, const char *src)
{
	char	*new_dest;
	size_t	dest_len;
	size_t	src_len;
	size_t	total_len;

	dest_len = 0;
	if (dest)
		dest_len = ft_strlen(dest);
	src_len = ft_strlen(src);
	total_len = dest_len + src_len;
	new_dest = (char *)malloc(total_len + 1);
	if (!new_dest)
	{
		free(dest);
		return (NULL);
	}
	if (dest)
		ft_strlcpy(new_dest, dest, dest_len + 1);
	else
		new_dest[0] = '\0';
	ft_strlcat(new_dest, src, total_len + 1);
	free(dest);
	return (new_dest);
}

typedef struct s_var_expand_state
{
	char	*expanded_str;
	int		i;
	char	in_single_quote;
	t_shell	*shell;
} t_var_expand_state;

static void	init_expand_state(t_var_expand_state *state, t_shell *shell_ptr)
{
	state->expanded_str = ft_strdup("");
	state->i = 0;
	state->in_single_quote = 0;
	state->shell = shell_ptr;
}

static int	hande_dollar_expansion(char *str, t_var_expand_state *state)
{
	char	var_name_buffer[256];
	int		j;
	char	*var_value;

	state->i++;
	j = 0;
	if (str[state->i] == '?')
		var_name_buffer[j++] = str[state->i++];
	else
	{
		while (str[state->i] && (ft_isalnum(str[state->i]) || str[state->i] == '_'))
		{
			if (j < 255)
				var_name_buffer[j++] = str[state->i++];
			else
				break ;
		}
	}
	var_name_buffer[j] = '\0';
	if (j > 0)
	{
		var_value = get_var_value(var_name_buffer, state->shell);
		if (var_value)
		{
			state->expanded_str = str_append_str(state->expanded_str, var_value);
			if (ft_strcmp(var_name_buffer, "?") == 0)
				free(var_value);
		}
	}
	else if (str[state->i - 1] == '$')
		state->expanded_str = str_append_char(state->expanded_str, '$');
	if (!state->expanded_str)
		return (0);
	return (1);
}

char	*expand_variables_in_str(char *str, t_shell *shell)
{
	t_var_expand_state state;

	if (!str)
		return (NULL);
	init_expand_state(&state, shell);
	if (!state.expanded_str)
		return (NULL);
	while (str[state.i])
	{
		if (str[state.i] == '\'')
		{
			state.in_single_quote = !state.in_single_quote;
			state.expanded_str = str_append_char(state.expanded_str, str[state.i]);
			state.i++;
		}
		else if (str[state.i] == '$' && !state.in_single_quote && str[state.i + 1])
		{
			if (!hande_dollar_expansion(str, &state))
				return (NULL);
		}
		else
		{
			state.expanded_str = str_append_char(state.expanded_str, str[state.i]);
			state.i++;
		}
		if (!state.expanded_str)
			return (NULL);
	}
	return (state.expanded_str);
} 