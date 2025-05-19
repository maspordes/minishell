/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:03:23 by shutan            #+#    #+#             */
/*   Updated: 2024/05/06 22:33:40 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (i < size - 1 && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen(src));
}

// int	main(void)
// {
// 	char			dest[10] = "Hello";
// 	char			src[20] = "World";
// 	unsigned int	n;

// 	n = 10;
// //	printf("%d", ft_strlcpy(&dest[2], &src[1], n));
// 	printf("%ld", strlcpy(dest, src, 10));
// 	return (0);
// }