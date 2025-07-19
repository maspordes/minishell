/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:03:03 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 21:04:40 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_env	*new_env(char *key, char *value)
{
	t_env	*env;

	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->key = ft_strdup(key);
	if (value)
		env->value = ft_strdup(value);
	else
		env->value = NULL;
	env->next = NULL;
	return (env);
}

void	add_env(t_env **env_list, t_env *new_env)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_env;
		return ;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_env;
}

void	parse_env(t_env **env_list, char *env_str)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	if (!env_str[i])
	{
		add_env(env_list, new_env(env_str, NULL));
		return ;
	}
	key = ft_substr(env_str, 0, i);
	value = ft_substr(env_str, i + 1, ft_strlen(env_str) - i - 1);
	add_env(env_list, new_env(key, value));
	free(key);
	free(value);
}

void	free_env(t_env *env_list)
{
	t_env	*tmp;

	while (env_list)
	{
		tmp = env_list;
		env_list = env_list->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
