/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:54:35 by shutan            #+#    #+#             */
/*   Updated: 2025/05/23 21:53:45 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <limits.h>

/*
** pwd 命令的实现
** Should always print the actual current working directory
*/
int	ft_pwd(void)
{
	char	cwd[PATH_MAX];
	char	*pwd_env;

	pwd_env = getenv("PWD");
	if (pwd_env)
	{
		printf("%s\n", pwd_env);
		return (0);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
}
