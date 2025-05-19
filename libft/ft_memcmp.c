/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 23:42:42 by shutan            #+#    #+#             */
/*   Updated: 2024/05/08 23:16:29 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

/* int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*tmp1;
	const unsigned char	*tmp2;

	tmp1 = (const unsigned char *)s1;
	tmp2 = (const unsigned char *)s2;
	if (!tmp1 && !tmp2)
		return (0);	
	while (*tmp1 == *tmp2 && n)
	{
		tmp1++;
		tmp2++;
		n--;
	}
	if (n == 0)
		return (0);
	return ((unsigned char)*tmp1 - (unsigned char)*tmp2);
} */
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*tmp1;
	const unsigned char	*tmp2;
	size_t				i;

	tmp1 = (const unsigned char *)s1;
	tmp2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (tmp1[i] != tmp2[i])
			return (tmp1[i] - tmp2[i]);
		i++;
	}
	return (0);
}
/*int	main(void)
{
	char	s1[] = "abc";
	char	s2[] = "abcd";
	printf("%d", ft_memcmp(s1, s2, 4));
	return (0);
}*/