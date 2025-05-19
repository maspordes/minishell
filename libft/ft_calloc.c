/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 21:48:38 by shutan            #+#    #+#             */
/*   Updated: 2024/05/08 18:03:07 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*tmp;
	size_t			i;

	i = 0;
	if (count != 0 && count * size / count != size)
		return (NULL);
	tmp = malloc(count * size);
	if (!tmp)
		return (NULL);
	while (i < size * count)
	{
		tmp[i] = 0;
		i++;
	}
	return (tmp);
}

/*int	main(void)
{
	size_t count = 5;
	size_t size = sizeof(int);
	int	*arr;

	arr = (int *)ft_calloc(count, size);
	if (arr0 == NULL)
	{
		printf("Memory allocation failed \n");
		return (1);
	}
	printf("Value of the allocated memory: %d %d %d %d %d\n", 
	arr[0], arr[1], arr[2], arr[3], arr[4]);
	free(arr);
	
	size_t count2 = 5;
	size_t size2 = sizeof(char);
	char	*s;

	s = (char *)ft_calloc(count2, size2);
	if (s == NULL)
	{
		printf("Memory allocation failed \n");
		return (1);
	}
	printf("Value of the allocated memory: %c%c%c%c%c\n",  
	s[0] + 48, s[1], s[2], s[3], s[4]);
	free(s);
}*/