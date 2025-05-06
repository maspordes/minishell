/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 00:46:15 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 从环境变量列表中删除指定的键 */
static void	remove_env(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;
	t_env	*to_free;

	if (!*env_list)
		return ;
	if (ft_strcmp((*env_list)->key, key) == 0)
	{
		to_free = *env_list;
		*env_list = (*env_list)->next;
		free(to_free->key);
		if (to_free->value)
			free(to_free->value);
		free(to_free);
		return ;
	}
	prev = *env_list;
	current = (*env_list)->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			prev->next = current->next;
			free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
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

/* unset 命令的实现 */
int	ft_unset(char **args, t_env **env_list)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	while (args[i])
	{
		if (validate_env_key(args[i]))
			remove_env(env_list, args[i]);
		else
		{
			printf("unset: '%s': not a valid identifier\n", args[i]);
			ret = 1;
		}
		i++;
	}
	return (ret);
} 