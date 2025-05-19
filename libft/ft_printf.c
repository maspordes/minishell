/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:45:30 by shutan         #+#    #+#             */
/*   Updated: 2025/02/13 16:49:02 by shutan        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_digit(int n)
{
	int		len;
	char	c;

	len = 0;
	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	if (n < 0)
	{
		len += write(1, "-", 1);
		n = -n;
	}
	if (n >= 10)
		len += print_digit(n / 10);
	c = n % 10 + '0';
	len += write(1, &c, 1);
	return (len);
}

static int	print_format(char spcf, va_list args)
{
	char	c;

	if (spcf == 'c')
	{
		c = va_arg(args, int);
		return (write(1, &c, 1));
	}
	else if (spcf == 's')
		return (print_str(va_arg(args, char *)));
	else if (spcf == 'p')
		return (print_ptr(va_arg(args, unsigned long int)));
	else if (spcf == 'd' || spcf == 'i')
		return (print_digit(va_arg(args, int)));
	else if (spcf == 'u')
		return (print_u(va_arg(args, unsigned int)));
	else if (spcf == 'x' || spcf == 'X')
		return (print_hex(va_arg(args, unsigned int), spcf));
	else if (spcf == '%')
		return (write(1, "%", 1));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		len;

	if (!format)
		return (-1);
	va_start(args, format);
	i = 0;
	len = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			len += print_format(format[i + 1], args);
			i++;
		}
		else
			len += write(1, &format[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}

// 删除或注释掉这部分
/*
int main()
{
	// 测试代码
	return 0;
}
*/