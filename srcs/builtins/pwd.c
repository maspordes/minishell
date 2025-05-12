/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:54:35 by shutan            #+#    #+#             */
/*   Updated: 2025/05/12 20:31:56 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** pwd 命令的实现
** Should always print the actual current working directory
*/
int	ft_pwd(void)
{
	char	cwd[PATH_MAX]; /* Use PATH_MAX for better buffer size */

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		/* Handle potential errors, e.g., path too long or permissions */
		perror("minishell: pwd");
		return (1);
	}
}
