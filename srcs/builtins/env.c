/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 13:31:44 by shutan            #+#    #+#             */
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
