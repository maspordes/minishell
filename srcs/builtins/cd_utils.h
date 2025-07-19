/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:31:27 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 20:32:02 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CD_UTILS_H
# define CD_UTILS_H

# include "../../includes/minishell.h"

char	*get_home_path(t_env *env_list);
char	*get_oldpwd_path(t_env *env_list);
char	*get_target_path(char **args, t_env *env_list);
int		handle_cd_error(char *target_path, char *old_pwd_val, char **args);
char	*get_old_pwd(t_env *env_list);

#endif
