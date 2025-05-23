/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:45:30 by marrey            #+#    #+#             */
/*   Updated: 2025/05/23 23:06:21 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**perform_single_expansion(char *token, t_shell *shell)
{
	char	*vars_expanded_token;
	char	*quotes_removed_token;
	char	**result_array;

	vars_expanded_token = expand_variables_in_str(token, shell);
	if (!vars_expanded_token)
		return (NULL);
	quotes_removed_token = remove_quotes_from_str(vars_expanded_token);
	free(vars_expanded_token);
	if (!quotes_removed_token)
		return (NULL);
	result_array = malloc(sizeof(char *) * 2);
	if (!result_array)
	{
		free(quotes_removed_token);
		return (NULL);
	}
	result_array[0] = quotes_removed_token;
	result_array[1] = NULL;
	return (result_array);
}

static void	free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static int	expand_redirects(t_cmd *cmd, t_shell *shell)
{
	t_redirect	*current;
	char		**expanded;

	current = cmd->redirects;
	while (current)
	{
		expanded = perform_single_expansion(current->file, shell);
		if (!expanded)
			return (-1);
		if (expanded[0] == NULL || expanded[1] != NULL
			|| *expanded[0] == '\0')
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(current->file, STDERR_FILENO);
			ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
			free_str_array(expanded);
			shell->exit_status = 1;
			return (-1);
		}
		free(current->file);
		current->file = expanded[0];
		free(expanded);
		current = current->next;
	}
	return (0);
}

static int	expand_args(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	**expanded;

	if (!cmd->args)
		return (0);
	i = 0;
	while (cmd->args[i])
	{
		expanded = perform_single_expansion(cmd->args[i], shell);
		if (!expanded)
			return (-1);
		free(cmd->args[i]);
		cmd->args[i] = expanded[0];
		free(expanded);
		i++;
	}
	return (0);
}

int	expand_command(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);
	if (expand_args(cmd, shell) != 0)
		return (-1);
	if (expand_redirects(cmd, shell) != 0)
		return (-1);
	return (0);
}
