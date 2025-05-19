/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 17:27:51 by shutan            #+#    #+#             */
/*   Updated: 2024/05/05 19:10:41 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	p = s;
	while (i < n)
	{
		p[i] = '\0';
		i++;
	}
}

/*int	main(void)
{
	char	s[100] = "Helloooooooo";
	printf("%s\n", s);
	bzero(&s[5], 1);
	printf("%s\n", s);
	printf("%s\n", &s[6]);// so interesting that i can do this!
	char	m[100] = "Helloooooooo";
	printf("%s\n", m);
	bzero(&m[5], 1);
	printf("%s\n", m);
	printf("%s", &m[6]);
	return (0);
}*/