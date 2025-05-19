/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:42:00 by shutan         #+#    #+#             */
/*   Updated: 2025/02/13 16:42:00 by shutan        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft.h"

int	ft_printf(const char *format, ...);
int	print_ptr(unsigned long int pa);
int	print_str(char *str);
int	print_u(int num);
int	print_hex(unsigned int num, char format);

#endif 