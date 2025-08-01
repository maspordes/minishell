/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 15:15:27 by shutan            #+#    #+#             */
/*   Updated: 2024/05/07 00:20:19 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

/*int	main(void)
{
	printf("%d\n", ft_isalnum('a'));
	printf("%d", isalnum('a'));
}*/