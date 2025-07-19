/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 22:03:57 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_args(char **args)
{
	int	size;

	size = 0;
	if (args)
		while (args[size])
			size++;
	return (size);
}

static char	**create_new_args(char **old_args, int size, char *new_arg)
{
	char	**new_args;
	int		i;

	new_args = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_args[i] = old_args[i];
		i++;
	}
	new_args[i] = ft_strdup(new_arg);
	new_args[i + 1] = NULL;
	return (new_args);
}

int	add_arg(t_cmd *cmd, char *arg)
{
	int		size;
	char	**new_args;

	size = count_args(cmd->args);
	new_args = create_new_args(cmd->args, size, arg);
	if (!new_args)
		return (0);
	if (cmd->args)
		free(cmd->args);
	cmd->args = new_args;
	return (1);
}

t_cmd	*handle_pipe(t_token **token, t_cmd *cmd)
{
	t_cmd	*new;

	*token = (*token)->next;
	new = new_cmd();
	if (!new)
		return (NULL);
	cmd->next = new;
	return (new);
}
