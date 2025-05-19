/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 21:03:03 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* env 命令的实现 */
int	ft_env(t_env *env_list)
{
	setvbuf(stdout, NULL, _IOLBF, 0);
	while (env_list)
	{
		if (env_list->value)
		{
			printf("%s=%s\n", env_list->key, env_list->value);
			fflush(stdout);
		}
		env_list = env_list->next;
	}
	return (0);
}
