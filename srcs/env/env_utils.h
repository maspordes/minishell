/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:03:03 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 21:04:08 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_UTILS_H
# define ENV_UTILS_H

# include "../../includes/minishell.h"

t_env	*new_env(char *key, char *value);
void	add_env(t_env **env_list, t_env *new_env);
void	parse_env(t_env **env_list, char *env_str);
void	free_env(t_env *env_list);

#endif
