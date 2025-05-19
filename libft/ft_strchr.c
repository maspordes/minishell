/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 21:15:44 by shutan            #+#    #+#             */
/*   Updated: 2024/05/09 17:15:10 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if ((unsigned char)c == 0)
	{
		return ((char *)s);
	}
	return (NULL);
}

/* int	main(void)
{
	char	s[] = "Hallo, World!";
	printf("%p", ft_strchr(s, 0));
	return (0);
} */