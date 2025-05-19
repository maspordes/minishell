/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:22:10 by shutan            #+#    #+#             */
/*   Updated: 2024/05/08 22:30:21 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	long int	num;

	num = n;
	if (num == -2147483648)
	{
		ft_putstr_fd("-2", fd);
		ft_putnbr_fd(147483648, fd);
		return ;
	}
	if (num < 0)
	{
		num = -n;
		ft_putchar_fd('-', fd);
	}
	if (num >= 10)
	{
		ft_putnbr_fd(num / 10, fd);
		ft_putnbr_fd(num % 10, fd);
	}
	else
		ft_putchar_fd(num + '0', fd);
}

/* int	main(void)
{
	ft_putnbr_fd(-2352323, 1);
} */