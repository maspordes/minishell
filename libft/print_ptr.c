/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:44:08 by shutan         #+#    #+#             */
/*   Updated: 2025/02/13 16:44:16 by shutan        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_ptr(unsigned long int pa)
{
	write(1, "0x", 2);
	return (print_hex(pa, 'x') + 2);
}

// int	main(void)
// {
// 	int	*a;
// 	int	A;
// 	A = 33;
// 	a = &A;
// 	printf("\n%d", putptr((unsigned long)a));
// 	printf("\n%p", a);
// 	return (0);
// }