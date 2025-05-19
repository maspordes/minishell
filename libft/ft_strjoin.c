/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 22:07:49 by shutan            #+#    #+#             */
/*   Updated: 2024/04/30 23:24:46 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

size_t	ft_strlen(char *str);

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	count1;
	size_t	count2;
	size_t	i;
	char	*tmp;

	i = 0;
	count1 = ft_strlen((char *)s1);
	count2 = ft_strlen((char *)s2);
	tmp = (char *)malloc(sizeof(char) * (count1 + count2) + 1);
	if (!tmp)
		return (NULL);
	while (i < count1)
	{
		tmp[i] = s1[i];
		i++;
	}
	i = 0;
	while (i < count2)
	{
		tmp[count1 + i] = s2[i];
		i++;
	}
	tmp[count1 + i] = '\0';
	return (tmp);
}

/*int	main(void)
{
	char const s1[] = "Hello, ";
	char const s2[] = "World";
	printf("%s", ft_strjoin(s1, s2));
	return (0);
}*/