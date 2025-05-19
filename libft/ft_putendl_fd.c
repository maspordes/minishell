/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:16:11 by shutan            #+#    #+#             */
/*   Updated: 2024/05/03 22:21:43 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putendl_fd(char *s, int fd)
{
	int	len;

	len = 0;
	while (s[len])
	{
		write(fd, &s[len], 1);
		len++;
	}
	write(fd, "\n", 1);
}

/* int	main(void)
{
	char	s[] = "Hallo";

	ft_putendl_fd(s, 1);
	return (0);
} */