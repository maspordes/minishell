/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 13:31:44 by shutan            #+#    #+#             */
/*   Updated: 2025/07/18 14:43:04 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_env_node(t_env *node)
{
	free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

static void	remove_first_env(t_env **env_list)
{
	t_env	*to_free;

	to_free = *env_list;
	*env_list = (*env_list)->next;
	free_env_node(to_free);
}

static void	remove_env(t_env **env_list, char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!*env_list)
		return ;
	if (ft_strcmp((*env_list)->key, key) == 0)
	{
		remove_first_env(env_list);
		return ;
	}
	prev = *env_list;
	current = (*env_list)->next;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			prev->next = current->next;
			free_env_node(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

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
			fprintf(stderr, "unset: '%s': not a valid identifier\n", args[i]);
			ret = 1;
		}
		i++;
	}
	return (ret);
}
