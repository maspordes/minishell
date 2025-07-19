/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_validation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 23:54:17 by shutan            #+#    #+#             */
/*   Updated: 2025/05/24 12:12:43 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "exit_utils.h"

static void	cleanup_clean_arg(char **clean_arg)
{
	if (*clean_arg)
	{
		free(*clean_arg);
		*clean_arg = NULL;
	}
}

static int	check_numeric_arg(char **args, char **clean_arg)
{
	if (!is_numeric_argument(args[1]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			args[1]);
		cleanup_clean_arg(clean_arg);
		return (255);
	}
	return (0);
}

static int	check_too_many_args(char **args, char **clean_arg)
{
	if (args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		cleanup_clean_arg(clean_arg);
		return (1);
	}
	return (0);
}

int	validate_exit_args(char **args, char **clean_arg)
{
	int	result;

	if (!args[1])
		return (0);
	*clean_arg = NULL;
	if (args[1][0] == '"' || args[1][0] == '\'')
	{
		*clean_arg = ft_substr(args[1], 1, ft_strlen(args[1]) - 2);
		if (!*clean_arg)
			return (255);
	}
	result = check_numeric_arg(args, clean_arg);
	if (result)
		return (result);
	return (check_too_many_args(args, clean_arg));
}
