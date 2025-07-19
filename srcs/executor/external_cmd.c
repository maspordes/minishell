/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:42:18 by shutan            #+#    #+#             */
/*   Updated: 2025/07/19 18:09:03 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*check_absolute_path(char *cmd)
{
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (access(cmd, F_OK) != 0)
			return (NULL);
		return (ft_strdup(cmd));
	}
	return (NULL);
}

static char	*search_in_path(char *cmd, char **paths)
{
	char	*cmd_path;
	char	*temp_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp_path = ft_strjoin(paths[i], "/");
		if (!temp_path)
			return (NULL);
		cmd_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, X_OK) == 0)
		{
			free_array(paths);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	return (NULL);
}

char	*find_executable(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*result;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	result = check_absolute_path(cmd);
	if (result)
		return (result);
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	result = search_in_path(cmd, paths);
	if (!result)
		free_array(paths);
	return (result);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	execute_external_cmd(t_cmd *cmd, t_env *env_list)
{
	char	*cmd_path;
	char	**env_array;

	if (!cmd->args || !cmd->args[0])
		return (127);
	cmd_path = find_executable(cmd->args[0], env_list);
	if (!cmd_path)
	{
		print_error("minishell", cmd->args[0], "command not found");
		return (127);
	}
	env_array = env_to_array(env_list);
	if (!env_array)
	{
		free(cmd_path);
		return (1);
	}
	if (execve(cmd_path, cmd->args, env_array) == -1)
	{
		print_error("minishell", cmd->args[0], strerror(errno));
		free(cmd_path);
		free_array(env_array);
		return (126);
	}
	return (0);
}
