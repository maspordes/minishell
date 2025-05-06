/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:54:45 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 00:54:45 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 打印所有环境变量（带 declare -x 前缀） */
static void	print_export_env(t_env *env_list)
{
	while (env_list)
	{
		printf("declare -x %s", env_list->key);
		if (env_list->value)
			printf("=\"%s\"", env_list->value);
		printf("\n");
		env_list = env_list->next;
	}
}

/* 解析待设置的环境变量 */
static int	parse_export_arg(char *arg, char **key, char **value)
{
	int	i;
	int	has_equal;

	i = 0;
	has_equal = 0;
	while (arg[i])
	{
		if (arg[i] == '=')
		{
			has_equal = 1;
			break ;
		}
		i++;
	}
	*key = ft_substr(arg, 0, i);
	if (has_equal)
		*value = ft_substr(arg, i + 1, ft_strlen(arg) - i - 1);
	else
		*value = NULL;
	return (1);
}

/* 验证环境变量名是否有效 */
static int	validate_env_key(char *key)
{
	int	i;

	if (!key || !key[0] || !(ft_isalpha(key[0]) || key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/* export 命令的实现 */
int	ft_export(char **args, t_env **env_list)
{
	int		i;
	char	*key;
	char	*value;
	int		ret;

	ret = 0;
	if (!args[1])
	{
		print_export_env(*env_list);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (parse_export_arg(args[i], &key, &value))
		{
			if (validate_env_key(key))
				set_env_value(env_list, key, value);
			else
			{
				printf("export: '%s': not a valid identifier\n", args[i]);
				ret = 1;
			}
			free(key);
			if (value)
				free(value);
		}
		i++;
	}
	return (ret);
}
