/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 19:09:31 by shutan            #+#    #+#             */
/*   Updated: 2024/05/05 20:38:59 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* char apply_function(unsigned int index, char c) 
{
	printf("Index: %u, Character: %c\n", index, c);
	return toupper(c);
}
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	len;
	size_t	i;
	char	*result;

	i = 0;
	len = 0;
	while (s[len])
		len++;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	while (i < len)
	{
		result[i] = f(i, s[i]);
		i++;
	}
	result[len] = '\0';
	return (result);
}

/* int main() {
    char *my_string = "hello world";
    char *new_string = ft_strmapi(my_string, apply_function);

    printf("Original string: %s\n", my_string);
    printf("New string: %s\n", new_string);

    free(new_string);
    return 0;
} */