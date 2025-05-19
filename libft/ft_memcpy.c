/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 18:03:59 by shutan            #+#    #+#             */
/*   Updated: 2024/05/08 22:59:19 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*p;
	const unsigned char	*s;

	if (!dest && !src)
		return (dest);
	i = 0;
	p = (unsigned char *)dest;
	s = (const unsigned char *)src;
	while (i < n)
	{
		p[i] = s[i];
		i++;
	}
	return (dest);
}
/* 
int	main(void)
{
	char	dest[] = "abcde";
	char	src[] = "12345";
	memcpy(&dest[3], &src[2], 1);
	char	dest2[] = "abcde";
	char	src2[] = "12345";
	ft_memcpy(&dest2[3], &src2[2], 2);
	int	arr1[] = {1, 2, 3, 4, 5};
	int	arr2[] = {6, 7, 8, 9, 10};
	memcpy(&arr1[1], &arr2[2], 8);
	printf("%s\n", dest);
	printf("%s\n", dest2);
	int	i;

	i = 0;
	while (i < 5)
	{
		printf("%d", arr1[i]);
		i++;
	}
	return (0);
}
 */