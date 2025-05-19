/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:25:24 by shutan            #+#    #+#             */
/*   Updated: 2024/05/08 23:19:20 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stddef.h>

size_t	ft_strlen(const char *str);

char	*strnstr(const char *big, const char *little, size_t len);

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	m;
	size_t	n;

	m = 0;
	n = 0;
	if (*little == '\0')
		return ((char *)big);
	if (little == NULL)
		return (NULL);
	if (big == NULL && len == 0)
		return (NULL);
	while (big[m] && m < len)
	{
		n = 0;
		while (big[m + n] == little[n] && little[n] && m + n < len)
			n++;
		if (little[n] == '\0')
			return ((char *)&big[m]);
		m++;
	}
	return (NULL);
}

/*int	main(void)
{
	const char	s1[] = "Hello, World! Nice to meet you!";
	const char	s2[] = "World";
	const char	s3[] = "";
	char	*res = ft_strnstr(s1, s2, 20);
	printf("%s\n", res);
	printf("%s", strnstr(s1, s3, 20));
	return(0);
}*/