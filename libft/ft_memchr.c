/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:29:20 by shutan            #+#    #+#             */
/*   Updated: 2024/05/07 23:50:14 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned const char	*ptr_s;
	size_t				i;

	i = 0;
	ptr_s = s;
	while (i < n)
	{
		if (ptr_s[i] == (unsigned char)c)
			return ((void *)&ptr_s[i]);
		i++;
	}
	return (NULL);
}

/*
Yes, that is correct. If you have a char *p that is pointing 
to a string, then p[1] is the value of the second character
 of the string that p is pointing to. And &p[1] is the address 
 of that second character, not the address of the pointer p itself.
Specifically:
p is a pointer to a character, which means it holds the address of 
the first character in a string. p[1] accesses the second character 
in the string that p is pointing to, because array indexing starts at 0.
&p[1] gives you the address of that second character, not the address of 
the pointer p.*/

/*int	main(void)
{
	char s1[] = "Hallo!";
	printf("%s", (char *)memchr(s1, 'a', 4));
	char s2[] = "Hallo!";
	printf("%s", (char *)ft_memchr(s2, 'a', 4));
	return (0);
}*/