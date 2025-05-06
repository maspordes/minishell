/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 00:00:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/05 00:46:15 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 释放字符串数组 */
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

/* 展开环境变量 */
char	*expand_variables(char *str, t_env *env_list, int exit_status)
{
	int		i;
	int		in_quotes;
	char	*result;
	char	*var_name;
	char	*var_value;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			in_quotes = !in_quotes;
		else if (str[i] == '$' && !in_quotes)
		{
			i++;
			if (str[i] == '?')
			{
				var_value = ft_itoa(exit_status);
				result = ft_strjoin(result, var_value);
				free(var_value);
				i++;
			}
			else if (ft_isalpha(str[i]) || str[i] == '_')
			{
				var_name = ft_strdup("");
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				{
					var_name = ft_stradd_char(var_name, str[i]);
					i++;
				}
				var_value = get_env_value(env_list, var_name);
				if (var_value)
					result = ft_strjoin(result, var_value);
				free(var_name);
			}
			else
				result = ft_stradd_char(result, '$');
		}
		else
			result = ft_stradd_char(result, str[i++]);
	}
	return (result);
} 