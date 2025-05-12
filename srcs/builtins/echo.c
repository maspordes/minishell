/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 00:43:53 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 判断是否为 -n 选项 */
static int	is_n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

/* 处理引号 */
static char	*handle_quotes(char *str, t_env *env_list)
{
	char	*result;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;
	char	*var_name;
	char	*var_value;
	char	*temp;
	char	chr[2];

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++;
			continue;
		}
		else if (str[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++;
			continue;
		}
		else if (str[i] == '$' && !in_single_quotes)
		{
			i++;
			if (str[i] == '?')
			{
				var_value = get_env_value(env_list, "?");
				if (var_value)
				{
					temp = result;
					result = ft_strjoin(result, var_value);
					free(temp);
				}
				else
				{
					temp = result;
					result = ft_strjoin(result, "0");
					free(temp);
				}
				i++;
			}
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				var_name = ft_strdup("");
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				{
					temp = var_name;
					chr[0] = str[i];
					chr[1] = '\0';
					var_name = ft_strjoin(var_name, chr);
					free(temp);
					i++;
				}
				var_value = get_env_value(env_list, var_name);
				if (var_value)
				{
					temp = result;
					result = ft_strjoin(result, var_value);
					free(temp);
				}
				free(var_name);
			}
			else
			{
				temp = result;
				chr[0] = '$';
				chr[1] = '\0';
				result = ft_strjoin(result, chr);
				free(temp);
			}
		}
		else
		{
			temp = result;
			chr[0] = str[i];
			chr[1] = '\0';
			result = ft_strjoin(result, chr);
			free(temp);
			i++;
		}
	}
	return (result);
}

/* echo 命令的实现 */
int	ft_echo(char **args, t_env *env_list)
{
	int		i;
	int		n_option;
	char	*processed_arg;

	n_option = 0;
	i = 1;
	while (args[i] && is_n_option(args[i]))
	{
		n_option = 1;
		i++;
	}
	while (args[i])
	{
		processed_arg = handle_quotes(args[i], env_list);
		if (processed_arg)
		{
			printf("%s", processed_arg);
			free(processed_arg);
		}
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_option)
		printf("\n");
	return (0);
}
