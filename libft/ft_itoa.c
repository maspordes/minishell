/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 17:53:37 by shutan            #+#    #+#             */
/*   Updated: 2024/05/08 22:51:21 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include <stdlib.h>

static int	ft_numlen(long n)
{
	int	len;

	len = (n <= 0);
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	nb;
	int		len;
	int		sign;

	nb = n;
	sign = 0;
	if (n < 0)
		sign = 1;
	if (sign != 0)
		nb = -(long)n;
	len = ft_numlen(nb);
	str = malloc(sizeof(char) * (len + sign + 1));
	if (!str)
		return (NULL);
	str[len + sign] = '\0';
	while (len--)
	{
		str[len + sign] = nb % 10 + '0';
		nb /= 10;
	}
	if (sign)
		str[0] = '-';
	return (str);
}

/* char	*ft_itoa(int nbr)
{
	int		i;
	char	*str;
	long	div;
	int		is_negative;
	long	n;

	n = nbr;
	i = 1;
	is_negative = 0;
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	div = n;
	while (div > 9)
	{
		div /= 10;
		i++;
	}
	str = (char *)malloc(i * sizeof(char) + is_negative + 1);
	if (!str)
		return (NULL);
	str[i + is_negative] = '\0';
	i--;
	while (n >= 0 && i >= 0)
	{
		str[i + is_negative] = n % 10 + '0';
		n /= 10;
		i--;
	}
	if (is_negative)
		str[0] = '-';
	return (str);
}
 */
/*int	main(void)
{
	int	i;char	*ft_itoa(int nbr)
{
	int		i;
	char	*str;
	long	div;
	int		is_negative;
	long	n;

	n = nbr;
	i = 1;
	is_negative = 0;
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	div = n;
	while (div > 9)
	{
		div /= 10;
		i++;
	}
	str = (char *)malloc(i * sizeof(char) + is_negative + 1);
	if (!str)
		return (NULL);
	str[i + is_negative] = '\0';
	i--;
	while (n >= 0 && i >= 0)
	{
		str[i + is_negative] = n % 10 + '0';
		n /= 10;
		i--;
	}char	*ft_itoa(int nbr)
{
	int		i;
	char	*str;
	long	div;
	int		is_negative;
	long	n;

	n = nbr;
	i = 1;
	is_negative = 0;
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	div = n;
	while (div > 9)
	{
		div /= 10;
		i++;
	}
	str = (char *)malloc(i * sizeof(char) + is_negative + 1);
	if (!str)
		return (NULL);
	str[i + is_negative] = '\0';
	i--;
	while (n >= 0 && i >= 0)
	{
		str[i + is_negative] = n % 10 + '0';
		n /= 10;
		i--;
	}
	if (is_negative)
	while (n >= 0 && i >= 0)
	{
		str[i + is_negative] = n % 10 + '0';
		n /= 10;
		i--;
	}
	if (is_negative)
		str[0] = '-';
	return (str);
}
	if (is_negative)	{
		str[i + is_negative] = n % 10 + '0';
		n /= 10;
		i--;
	}
	if (is_negative)
	while (n >= 0 && i >= 0)
	{
		str[i + is_negative] = n % 10 + '0';
		n /= 10;
		i--;
	}
	if (is_negative)
		str[0] = '-';
	return (str);
}
	if (is_negative)
		str[0] = '-';
	return (str);
}
		str[0] = '-';
	return (str);
}
	if (is_negative)
		str[0] = '-';
	return (str);
}

	i = -123532;
	printf("%s", ft_itoa(i));
	return (0);
}*/