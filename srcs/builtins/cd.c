/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:46:46 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 00:46:48 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 更新 PWD 和 OLDPWD 环境变量 */
static void	update_pwd_env(t_env **env_list)
{
	char	cwd[1024];
	char	*old_pwd;

	old_pwd = get_env_value(*env_list, "PWD");
	if (old_pwd)
		set_env_value(env_list, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_value(env_list, "PWD", cwd);
}

/* 处理 cd 到 HOME 目录 */
static int	cd_to_home(t_env **env_list)
{
	char	*home_path;

	home_path = get_env_value(*env_list, "HOME");
	if (!home_path)
	{
		printf("cd: HOME not set\n");
		return (1);
	}
	if (chdir(home_path) != 0)
	{
		perror("cd");
		return (1);
	}
	update_pwd_env(env_list);
	return (0);
}

/* cd 命令的实现 */
int	ft_cd(char **args, t_env **env_list)
{
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
		return (cd_to_home(env_list));
	if (chdir(args[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	update_pwd_env(env_list);
	return (0);
} 