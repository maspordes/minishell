/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:22:56 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 21:07:00 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "env_utils.h"

t_env	*init_env(char **envp)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		parse_env(&env_list, envp[i]);
		i++;
	}
	return (env_list);
}

static char	*create_env_string(t_env *current)
{
	char	*tmp;
	char	*result;

	if (current->value)
	{
		tmp = ft_strjoin(current->key, "=");
		result = ft_strjoin(tmp, current->value);
		free(tmp);
		return (result);
	}
	return (ft_strdup(current->key));
}

char	**env_to_array(t_env *env_list)
{
	int		count;
	char	**env_array;
	t_env	*current;
	int		i;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = env_list;
	while (current)
	{
		env_array[i] = create_env_string(current);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
