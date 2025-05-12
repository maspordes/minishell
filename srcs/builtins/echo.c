/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/12 22:19:03 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 判断是否为 -n 选项 */
static int	is_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/* echo 命令的实现 */
int	ft_echo(char **args, t_env *env_list)
{
	int		i;
	int		n_option;

	(void)env_list; /* env_list is no longer needed if we don't expand here */
	n_option = 0;
	i = 1;
	while (args[i] && is_n_option(args[i]))
	{
		n_option = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]); /* Print the argument directly */
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_option)
		printf("\n");
	return (0);
}
