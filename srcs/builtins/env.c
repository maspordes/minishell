/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 00:43:53 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* env 命令的实现 */
int	ft_env(t_env *env_list)
{
	// Set stdout to line buffered mode for pipe operations
	setvbuf(stdout, NULL, _IOLBF, 0);

	while (env_list)
	{
		if (env_list->value)
		{
			// Ensure consistent format: KEY=VALUE\n
			printf("%s=%s\n", env_list->key, env_list->value);
			fflush(stdout);  // Ensure output is flushed for pipe operations
		}
		env_list = env_list->next;
	}
	return (0);
}
