/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:37:24 by shutan            #+#    #+#             */
/*   Updated: 2024/05/05 19:09:39 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	p = s;
	while (i < n)
	{
		p[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

/*int	main(void)
{
	char	s[100];
	s[99] = '\0';
	memset(s, '1', 10);
	printf("%s\n",s);
	ft_memset(s, 33, 5);
	printf("%s", s);
	return (0);
}*/

/*int	main(void)
{
	char	str1[20] = "Hello world!";
	ft_memcpy(&str1[2], &str1[1], 7);
	return (0);
}*/