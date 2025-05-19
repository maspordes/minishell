/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 20:37:15 by shutan            #+#    #+#             */
/*   Updated: 2025/05/19 20:38:01 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_UTILS_H
# define EXIT_UTILS_H

# include "../../includes/minishell.h"

char	*clean_quoted_str(const char *str);
int		check_numeric_chars(const char *str);
int		is_numeric_argument(const char *str);
long	ft_atol(const char *str);
char	*get_key_from_str(const char *str);
int		validate_exit_args(char **args, char **clean_arg);
int		handle_exit_args(char **args, long *exit_code);

#endif
