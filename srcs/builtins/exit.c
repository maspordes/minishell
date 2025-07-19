/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:54:57 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 15:58:06 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "exit_utils.h"

int	ft_exit(char **args, t_shell *shell)
{
	long	exit_code;
	int		result;

	(void)shell;
	if (shell->cmd_list && !shell->cmd_list->next)
		printf("exit\n");
	result = handle_exit_args(args, &exit_code);
	if (result == 1)
		return (1);
	if (result == 255)
		return (255);
	return ((int)exit_code);
}

char	*clean_quoted_str(const char *str)
{
	int		len;
	char	*clean_str;

	len = ft_strlen(str);
	if ((str[0] == '"' || str[0] == '\'') && str[len - 1] == str[0])
	{
		clean_str = ft_substr(str, 1, len - 2);
		if (!clean_str)
			return (NULL);
		return (clean_str);
	}
	return (NULL);
}

int	check_numeric_chars(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}
