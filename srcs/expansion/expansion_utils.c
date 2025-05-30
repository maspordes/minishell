/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/24 12:38:35 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**perform_single_expansion(char *token, t_shell *shell)
{
	char	*vars_expanded_token;
	char	*quotes_removed_token;

	vars_expanded_token = expand_variables_in_str(token, shell);
	if (!vars_expanded_token)
		return (NULL);
	quotes_removed_token = remove_quotes_from_str(vars_expanded_token);
	free(vars_expanded_token);
	if (!quotes_removed_token)
		return (NULL);
	if (*quotes_removed_token == '\0')
	{
		free(quotes_removed_token);
		return (create_empty_result());
	}
	return (create_single_result(quotes_removed_token));
}

int	process_single_arg(char **new_args, int *j, char *arg, t_shell *shell)
{
	char	**expanded;

	expanded = perform_single_expansion(arg, shell);
	if (!expanded)
		return (-1);
	if (expanded[0] != NULL)
	{
		new_args[*j] = expanded[0];
		(*j)++;
	}
	free(expanded);
	return (0);
}

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
