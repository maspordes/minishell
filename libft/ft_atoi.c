/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 17:11:36 by shutan            #+#    #+#             */
/*   Updated: 2024/05/06 23:50:16 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

int	ft_isdigit(unsigned char c);

int	ft_atoi(const char *str)
{
	int	multi;
	int	convert;

	multi = 1;
	convert = 0;
	while ((*str >= '\t' && *str <= '\r') || *str == ' ')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
		{
			multi *= -1;
			str++;
		}
		else if (*str == '+')
			str++;
	}
	while (ft_isdigit(*str) == 1)
	{
		convert = convert * 10 + *str - 48;
		str++;
	}
	return (convert * multi);
}

/*int	main(void)
{
	char	s[] = "\n524dgd";
	printf("%d\n", atoi(s));
	printf("%d", ft_atoi(s));
	return (0);
}*/