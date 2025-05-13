/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:45:30 by marrey            #+#    #+#             */
/*   Updated: 2025/05/13 16:08:24 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Forward declarations no longer needed here if defined in header
// char	*expand_variables_in_str(char *str, t_shell *shell);
// char	*remove_quotes_from_str(char *str);

/*
 * Function: perform_single_expansion
 * ----------------------------------
 * Performs variable expansion and quote removal on a single string token.
 * Does NOT yet handle field splitting or globbing.
 *
 * token: The original string token (e.g., an argument or filename).
 * shell: Pointer to the shell structure for context (env, exit status).
 *
 * returns: A dynamically allocated array containing ONE string (the result),
 *          or NULL on failure.
 */
static char	**perform_single_expansion(char *token, t_shell *shell)
{
	char	*vars_expanded_token;
	char	*quotes_removed_token;
	char	**result_array;

	// 1. Variable Expansion (respecting quotes)
	vars_expanded_token = expand_variables_in_str(token, shell);
	if (!vars_expanded_token)
		return (NULL);

	// 2. Quote Removal
	quotes_removed_token = remove_quotes_from_str(vars_expanded_token);
	free(vars_expanded_token); // Free intermediate string
	if (!quotes_removed_token)
		return (NULL);

	// --- TODO: Field Splitting & Globbing would go here ---

	// Create the result array (currently always size 1 + NULL)
	result_array = malloc(sizeof(char *) * 2);
	if (!result_array)
	{
		free(quotes_removed_token);
		return (NULL);
	}
	result_array[0] = quotes_removed_token; // Transfer ownership
	result_array[1] = NULL;
	return (result_array);
}

// Helper: Reallocate a string array (ft_realloc equivalent for char**)
// Assumes old_size is the number of elements *including* the old NULL terminator
static char	**realloc_str_array(char **old_array, size_t old_count, size_t new_cap)
{
	char	**new_array;
	size_t	i;

	new_array = malloc(sizeof(char *) * new_cap);
	if (!new_array)
		return (NULL);
	i = 0;
	if (old_array)
	{
		while (i < old_count && old_array[i] != NULL)
		{
			new_array[i] = old_array[i];
			i++;
		}
	}
	while (i < new_cap)
	{
		new_array[i] = NULL;
		i++;
	}
	free(old_array);
	return (new_array);
}

// Helper: Free a string array
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

typedef struct s_expand_args_state
{
	char	**new_args;
	int		old_idx;
	int		new_count;
	int		new_cap;
	t_shell *shell;
} t_expand_args_state;

static int append_expanded_arg(char **expanded, t_expand_args_state *state)
{
	int expanded_idx;

	expanded_idx = 0;
	while (expanded[expanded_idx])
	{
		if (state->new_count >= state->new_cap - 1)
		{
			state->new_cap *= 2;
			state->new_args = realloc_str_array(state->new_args,
						state->new_count + 1, state->new_cap);
			if (!state->new_args)
			{
				free_str_array(expanded);
				return (-1);
			}
		}
		state->new_args[state->new_count++] = expanded[expanded_idx];
		expanded_idx++;
	}
	free(expanded);
	return (0);
}

/*
 * Function: expand_args
 * ---------------------
 * Expands each argument in the command's argument list.
 * Handles the complexity that one argument might expand into multiple
 * (though current perform_single_expansion only returns one).
 * Replaces the original args array with a new one.
 *
 * cmd:   Pointer to the command structure.
 * shell: Pointer to the shell structure.
 *
 * returns: 0 on success, -1 on failure.
 */
static int expand_args(t_cmd *cmd, t_shell *shell)
{
	t_expand_args_state state;
	char				**current_args;
	char				**expanded_single;

	current_args = cmd->args;
	if (!current_args)
		return (0);
	state.new_cap = 10; /* Initial capacity for new_args */
	state.new_args = realloc_str_array(NULL, 0, state.new_cap);
	if (!state.new_args)
		return (-1);
	state.new_count = 0;
	state.old_idx = 0;
	state.shell = shell;
	while (current_args[state.old_idx])
	{
		expanded_single = perform_single_expansion(current_args[state.old_idx], state.shell);
		if (!expanded_single)
		{
			free_str_array(state.new_args);
			return (-1);
		}
		/* If an argument expanded to an empty string, and it wasn't originally */
		/* an explicit quoted empty string (like "" or ''), discard it. */
		if (expanded_single[0] && *expanded_single[0] == '\0' &&
		    (ft_strcmp(current_args[state.old_idx], "\"\"") != 0 &&
		     ft_strcmp(current_args[state.old_idx], "''") != 0))
		{
			free_str_array(expanded_single); /* Discard and free the ["", NULL] */
		}
		else
		{
			if (append_expanded_arg(expanded_single, &state) != 0)
			{
				free_str_array(state.new_args); /* Ensure cleanup on error */
				return (-1);
			}
		}
		state.old_idx++;
	}
	state.new_args[state.new_count] = NULL;
	free_str_array(cmd->args); /* Free the old args array */
	cmd->args = state.new_args;
	return (0);
}

/*
 * Function: expand_redirects
 * --------------------------
 * Expands the filename for each redirection.
 * If expansion results in multiple filenames (e.g., via globbing or splitting),
 * it's an "ambiguous redirect" error.
 *
 * cmd:   Pointer to the command structure.
 * shell: Pointer to the shell structure.
 *
 * returns: 0 on success, -1 on failure (memory error or ambiguous redirect).
 */
static int expand_redirects(t_cmd *cmd, t_shell *shell)
{
	t_redirect	*current;
	char		**expanded;

	current = cmd->redirects;
	while (current)
	{
		expanded = perform_single_expansion(current->file, shell);
		if (!expanded)
			return (-1);
		if (expanded[0] == NULL || expanded[1] != NULL || *expanded[0] == '\0')
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

/*
 * Function: expand_command
 * ------------------------
 * Main entry point for expanding a parsed command structure.
 * Performs expansion on arguments and redirection filenames.
 * Modifies the t_cmd structure in place.
 *
 * cmd:   Pointer to the command structure to expand.
 * shell: Pointer to the shell structure for context.
 *
 * returns: 0 on success, -1 on failure.
 */
int expand_command(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return (0);

	// Expand redirection filenames first. Ambiguous redirect is fatal.
	if (expand_redirects(cmd, shell) != 0)
	{
		return (-1);
	}

	// Expand arguments (command name is args[0])
	if (expand_args(cmd, shell) != 0)
	{
		return (-1);
	}

	// If args became empty after expansion (e.g., cmd was "$EMPTY"),
	// the executor needs to handle cmd->args == NULL or cmd->args[0] == NULL.

	return (0);
} 