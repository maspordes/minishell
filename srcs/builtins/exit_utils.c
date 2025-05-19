/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:37:15 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 20:41:02 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "exit_utils.h"

int	is_numeric_argument(const char *str)
{
	char	*clean_str;

	if (!str || !*str)
		return (0);
	clean_str = clean_quoted_str(str);
	if (clean_str)
	{
		str = clean_str;
		if (!check_numeric_chars(str))
		{
			free(clean_str);
			return (0);
		}
		free(clean_str);
		return (1);
	}
	return (check_numeric_chars(str));
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str && ft_isdigit(*str))
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

char	*get_key_from_str(const char *str)
{
	int		i;
	char	*key;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	key = ft_substr(str, 0, i);
	return (key);
}

int	validate_exit_args(char **args, char **clean_arg)
{
	if (!args[1])
		return (0);
	*clean_arg = NULL;
	if (args[1][0] == '"' || args[1][0] == '\'')
	{
		*clean_arg = ft_substr(args[1], 1, ft_strlen(args[1]) - 2);
		if (!*clean_arg)
			return (255);
	}
	if (!is_numeric_argument(args[1]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n",
			args[1]);
		free(*clean_arg);
		return (2);
	}
	if (args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		free(*clean_arg);
		return (1);
	}
	return (0);
}

int	handle_exit_args(char **args, long *exit_code)
{
	char	*clean_arg;
	int		result;

	result = validate_exit_args(args, &clean_arg);
	if (result)
		return (result);
	if (clean_arg)
		*exit_code = ft_atol(clean_arg);
	else
		*exit_code = ft_atol(args[1]);
	free(clean_arg);
	if (*exit_code < 0 || *exit_code > 255)
		*exit_code %= 256;
	if (*exit_code < 0)
		*exit_code += 256;
	return (0);
}
