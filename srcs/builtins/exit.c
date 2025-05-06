/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:54:57 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 13:43:17 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 检查字符串是否为有效的整数 */
static int	is_valid_number(const char *str)
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

/* exit 命令的实现 */
int	ft_exit(char **args, t_shell *shell)
{
	int	exit_code;

	printf("exit\n");
	if (!args[1])
	{
		free_shell(shell);
		exit(shell->exit_status);
	}
	if (!is_valid_number(args[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", args[1]);
		free_shell(shell);
		exit(255);
	}
	if (args[2])
	{
		printf("minishell: exit: too many arguments\n");
		return (1);
	}
	exit_code = ft_atoi(args[1]) % 256;
	free_shell(shell);
	exit(exit_code);
	return (0);
}
