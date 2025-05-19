/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:05:25 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 21:06:54 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "env_utils.h"

static void	update_existing_env(t_env *current, char *value)
{
	free(current->value);
	if (value)
		current->value = ft_strdup(value);
	else
		current->value = NULL;
}

static void	create_new_env_node(t_env **env_list, char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(key);
		return ;
	}
	new_node->key = key;
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = *env_list;
	*env_list = new_node;
}

void	add_or_update_env(t_env **env_list, const char *arg)
{
	char	*key;
	char	*value;
	t_env	*current;

	key = ft_strdup(arg);
	value = ft_strchr(key, '=');
	if (value)
	{
		*value = '\0';
		value++;
	}
	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			update_existing_env(current, value);
			free(key);
			return ;
		}
		current = current->next;
	}
	create_new_env_node(env_list, key, value);
}

char	*get_env_value(t_env *env_list, const char *key)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

void	set_env_value(t_env **env_list, char *key, char *value)
{
	t_env	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (current->value)
				free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			return ;
		}
		current = current->next;
	}
	add_env(env_list, new_env(key, value));
}
