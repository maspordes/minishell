/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 21:46:57 by marrey            #+#    #+#             */
/*   Updated: 2025/05/23 23:06:21 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variables_in_str(char *str, t_shell *shell)
{
	if (!str || !shell)
		return (NULL);
	return (expand_variables(str, shell->env_list, shell->exit_status));
}
