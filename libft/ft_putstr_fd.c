/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shutan <shutan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 22:12:29 by shutan            #+#    #+#             */
/*   Updated: 2024/05/03 22:15:38 by shutan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putstr_fd(char *s, int fd)
{
	int	len;

	len = 0;
	while (s[len])
	{
		write(fd, &s[len], 1);
		len++;
	}
}

/* int	main(void)
{
	char	s[] = "Hallo";

	ft_putstr_fd(s, 1);
	return (0);
} */