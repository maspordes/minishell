/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:26:43 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 20:32:07 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "cd_utils.h"

char	*get_home_path(t_env *env_list)
{
	char	*env_val;
	char	*target_path;

	env_val = get_env_value(env_list, "HOME");
	if (!env_val)
	{
		fprintf(stderr, "minishell: cd: HOME not set\n");
		return (NULL);
	}
	target_path = ft_strdup(env_val);
	return (target_path);
}

char	*get_oldpwd_path(t_env *env_list)
{
	char	*env_val;
	char	*target_path;

	env_val = get_env_value(env_list, "OLDPWD");
	if (!env_val)
	{
		fprintf(stderr, "minishell: cd: OLDPWD not set\n");
		return (NULL);
	}
	target_path = ft_strdup(env_val);
	printf("%s\n", target_path);
	return (target_path);
}

char	*get_target_path(char **args, t_env *env_list)
{
	char	*path_arg;
	char	*target_path;

	path_arg = args[1];
	if (!path_arg)
		target_path = get_home_path(env_list);
	else if (ft_strcmp(path_arg, "-") == 0)
		target_path = get_oldpwd_path(env_list);
	else
		target_path = ft_strdup(path_arg);
	if (!target_path)
		perror("cd: memory allocation error");
	return (target_path);
}

int	handle_cd_error(char *target_path, char *old_pwd_val, char **args)
{
	fprintf(stderr, "minishell: cd: ");
	if (args[1])
		fprintf(stderr, "%s: ", args[1]);
	else
		fprintf(stderr, "(HOME): ");
	perror(NULL);
	free(target_path);
	free(old_pwd_val);
	return (1);
}

char	*get_old_pwd(t_env *env_list)
{
	char	*old_pwd_val;

	old_pwd_val = get_env_value(env_list, "PWD");
	if (old_pwd_val)
		old_pwd_val = ft_strdup(old_pwd_val);
	return (old_pwd_val);
}
