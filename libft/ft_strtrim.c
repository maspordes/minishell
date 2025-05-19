/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 22:29:23 by shutan            #+#    #+#             */
/*   Updated: 2024/05/09 21:32:59 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_is_in_set(char c, char const *set)
{
	while (*set)
		if (c == *set++)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_is_in_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_is_in_set(s1[end - 1], set))
		end--;
	str = malloc(end - start + 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1 + start, end - start + 1);
	return (str);
}

/* int	main(void)
{
	char const s1[] = "Hallo, World!";
	char const s2[] = "lHmdmm,ao";
	printf("%s", ft_strtrim(s1, s2));
	return (0);
} */