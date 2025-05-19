/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:05:25 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 21:07:08 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_OPERATIONS_H
# define ENV_OPERATIONS_H

# include "../../includes/minishell.h"
# include "env_utils.h"

void	add_or_update_env(t_env **env_list, const char *arg);
char	*get_env_value(t_env *env_list, const char *key);
void	set_env_value(t_env **env_list, char *key, char *value);

#endif
