/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 23:11:05 by shutan            #+#    #+#             */
/*   Updated: 2024/05/09 17:18:52 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdio.h>

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s) - 1;
	if ((unsigned char)c == 0)
		return ((char *)(s + ft_strlen(s)));
	while (i >= 0)
	{
		if (*(unsigned char *)(s + i) == (unsigned char)c)
			return ((char *)(s + i));
		i--;
	}
	return (NULL);
}

/*int	main(void)
{
	char	s1[] = "Hallo, World!";
	printf("%s", ft_strrchr(s1, 'o'));
	return(0);
}*/