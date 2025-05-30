/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_memory.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 12:35:00 by shutan            #+#    #+#             */
/*   Updated: 2025/05/24 12:37:59 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**create_empty_result(void)
{
	char	**result_array;

	result_array = malloc(sizeof(char *) * 1);
	if (!result_array)
		return (NULL);
	result_array[0] = NULL;
	return (result_array);
}

char	**create_single_result(char *str)
{
	char	**result_array;

	result_array = malloc(sizeof(char *) * 2);
	if (!result_array)
	{
		free(str);
		return (NULL);
	}
	result_array[0] = str;
	result_array[1] = NULL;
	return (result_array);
}

void	free_str_array(char **array)
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

void	cleanup_args_on_error(char **new_args, int j)
{
	while (j > 0)
		free(new_args[--j]);
	free(new_args);
}

char	*expand_variables(char *str, t_env *env_list, int exit_status)
{
	char			*expanded;
	t_expand_data	data;
	t_expand_state	state;

	data.env_list = env_list;
	data.exit_status = exit_status;
	state.i = 0;
	state.in_single_quote = 0;
	state.in_double_quote = 0;
	expanded = ft_strdup("");
	return (process_expansion_loop(str, expanded, &data, &state));
}
