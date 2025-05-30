/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:45:30 by marrey            #+#    #+#             */
/*   Updated: 2025/05/24 12:31:41 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static char	**allocate_new_args(int arg_count)
{
	char	**new_args;

	new_args = malloc(sizeof(char *) * (arg_count + 1));
	return (new_args);
}

static int	process_args_loop(t_cmd *cmd, char **new_args, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd->args[i])
	{
		if (process_single_arg(new_args, &j, cmd->args[i], shell) == -1)
		{
			cleanup_args_on_error(new_args, j);
			return (-1);
		}
		free(cmd->args[i]);
		i++;
	}
	new_args[j] = NULL;
	return (0);
}

static int	expand_args(t_cmd *cmd, t_shell *shell)
{
	char	**new_args;
	int		arg_count;

	if (!cmd->args)
		return (0);
	arg_count = 0;
	while (cmd->args[arg_count])
		arg_count++;
	new_args = allocate_new_args(arg_count);
	if (!new_args)
		return (-1);
	if (process_args_loop(cmd, new_args, shell) == -1)
		return (-1);
	free(cmd->args);
	cmd->args = new_args;
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
