/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 20:30:05 by shutan            #+#    #+#             */
/*   Updated: 2024/05/08 23:33:41 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;

	if (!dest && !src)
		return (dest);
	d = (char *)dest;
	s = (const char *)src;
	if (d < s)
		while (n--)
			*d++ = *s++;
	else
	{
		d += n;
		s += n;
		while (n--)
			*--d = *--s;
	}
	return (dest);
}

//this main function below to test the difference between memcpy and memmove

/*int	main(void)
{
	char	str1[20] = "Hello world!";
	memcpy(&str1[2], &str1[1], 7);
	printf("Result: %s\n", str1);
	char	str2[20] = "Hello world!";
	memmove(&str2[2], &str2[1], 7);
	printf("Result: %s\n", str2);
    char	str3[20] = "Hello world!";
    ft_memmove(&str3[2], &str3[1], 7);
    printf("Result: %s\n", str3);
	return (0);
}*/
