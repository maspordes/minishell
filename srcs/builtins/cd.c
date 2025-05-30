/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:46:46 by shutan            #+#    #+#             */
/*   Updated: 2025/05/24 12:12:43 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "cd_utils.h"

static int	update_pwd_env(t_env **env_list, char *old_pwd_val)
{
	char	*new_pwd_val;

	new_pwd_val = getcwd(NULL, 0);
	if (!new_pwd_val)
	{
		perror("cd: getcwd error");
		if (old_pwd_val)
			set_env_value(env_list, "OLDPWD", old_pwd_val);
		return (1);
	}
	if (old_pwd_val)
		set_env_value(env_list, "OLDPWD", old_pwd_val);
	set_env_value(env_list, "PWD", new_pwd_val);
	free(new_pwd_val);
	return (0);
}

int	ft_cd(char **args, t_env **env_list)
{
	char	*target_path;
	char	*old_pwd_val;
	int		result;

	if (args[1] && args[2])
	{
		fprintf(stderr, "minishell: cd: too many arguments\n");
		return (0);
	}
	old_pwd_val = get_old_pwd(*env_list);
	target_path = get_target_path(args, *env_list);
	if (!target_path)
	{
		free(old_pwd_val);
		return (1);
	}
	if (chdir(target_path) != 0)
		return (handle_cd_error(target_path, old_pwd_val, args));
	result = update_pwd_env(env_list, old_pwd_val);
	free(target_path);
	free(old_pwd_val);
	return (result);
}
