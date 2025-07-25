/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 20:31:47 by shutan            #+#    #+#             */
/*   Updated: 2024/04/26 22:19:34 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_toupper(int c)
{
	if ((c >= 'a') && (c <= 'z'))
	{
		c -= 32;
	}
	return (c);
}

/*int	main(void)
{
	printf("%c", toupper('x'));
	printf("%c", ft_toupper('S'));
	return (0);
}*/