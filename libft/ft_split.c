/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 23:25:43 by shutan            #+#    #+#             */
/*   Updated: 2024/05/19 22:07:00 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static void	ft_freeall(char **arr, int i)
{
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
}

static char	**ft_populate_arr(char const *s, char c, char **arr, int count)
{
	int	i;
	int	len;
	int	start;

	i = 0;
	while (*s && i < count)
	{
		while (*s == c)
			s++;
		start = (int)(s - (char *)s);
		len = 0;
		while (s[len] && s[len] != c)
			len++;
		arr[i] = ft_substr((char *)s, start, len);
		if (!arr[i])
		{
			ft_freeall(arr, i - 1);
			return (NULL);
		}
		i++;
		s += len;
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		count;

	count = ft_count_words(s, c);
	arr = malloc((count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	return (ft_populate_arr(s, c, arr, count));
}

/* int	main(void)
{
	int		i;
	char	**tmp;
	char	s[] = "-aab---cdgewa---d-";

	i = 0;
	tmp = ft_split(s, '-');
	while (tmp[i] != NULL) 
	{
		printf("%s\n", tmp[i]);
		free(tmp[i]);
		i++;
	}
	free(tmp);
 	return (0);
} */