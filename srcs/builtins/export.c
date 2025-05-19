/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 00:54:45 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 20:56:45 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_export_env(t_env *env_list)
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

int	is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_export_arg(char *arg, t_env **env_list)
{
	char	*key;
	char	*value;

	key = ft_strdup(arg);
	value = ft_strchr(key, '=');
	if (value)
	{
		*value = '\0';
		value++;
	}
	if (!is_valid_identifier(key))
	{
		fprintf(stderr, "not a valid identifier\n");
		free(key);
		return (1);
	}
	add_or_update_env(env_list, arg);
	free(key);
	return (0);
}

int	ft_export(char **args, t_env **env_list)
{
	int	i;
	int	exit_status;

	if (!args[1])
	{
		print_export_env(*env_list);
		return (0);
	}
	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (handle_export_arg(args[i], env_list))
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
