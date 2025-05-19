/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 21:18:58 by shutan            #+#    #+#             */
/*   Updated: 2024/04/30 21:53:12 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

size_t	ft_strlen(char *str);

char	*ft_strdup(const char *s1)
{
	size_t	count;
	size_t	i;
	char	*tmp;

	i = 0;
	count = ft_strlen((char *)s1);
	tmp = (char *)malloc(sizeof(char) * count + 1);
	if (!tmp)
		return (NULL);
	while (s1[i] != '\0')
	{
		tmp[i] = s1[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

/*int	main(void)
{
	const char	s1[] = "Hallo, World!";
	printf("The allocated copy string is: %s\n", ft_strdup(s1));
	printf("The address of the original string is %p \n", s1);
	printf("The address of the allocated string is %p", ft_strdup(s1));
	return (0);
}*/