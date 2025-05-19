/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 21:34:48 by shutan            #+#    #+#             */
/*   Updated: 2024/05/09 16:04:21 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*tmp;
	size_t	lens;
	size_t	i;
	size_t	j;

	lens = ft_strlen(s);
	if (start >= lens)
		return (ft_calloc(1, 1));
	if (len > lens - start)
		len = lens - start;
	tmp = (char *)malloc(len + 1);
	if (!tmp)
		return (NULL);
	i = -1;
	j = start;
	while (++i < len)
		tmp[i] = s[j++];
	tmp[i] = '\0';
	return (tmp);
}

/*int	main(void)
{
	char	s[] = "Hallo, World!";
	printf("%s", ft_substr(s, 3, 5));
	return (0);
}*/