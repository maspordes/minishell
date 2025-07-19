/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marrey <marrey@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:30:00 by marrey            #+#    #+#             */
/*   Updated: 2025/07/19 19:51:31 by marrey           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*result;
	int		i;
	int		j;

	if (!s1)
	{
		s1 = malloc(1);
		if (!s1)
			return (NULL);
		s1[0] = '\0';
	}
	result = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		result[i++] = s2[j++];
	result[i] = '\0';
	free(s1);
	return (result);
}

static char	*ft_strchr_gnl(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

static char	*extract_line(char *saved)
{
	char	*line;
	int		i;

	if (!saved || !saved[0])
		return (NULL);
	i = 0;
	while (saved[i] && saved[i] != '\n')
		i++;
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (saved[i] && saved[i] != '\n')
	{
		line[i] = saved[i];
		i++;
	}
	if (saved[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*save_remainder(char *saved)
{
	char	*remainder;
	int		i;
	int		j;

	if (!saved)
		return (NULL);
	i = 0;
	while (saved[i] && saved[i] != '\n')
		i++;
	if (!saved[i])
	{
		free(saved);
		return (NULL);
	}
	remainder = malloc(ft_strlen(saved) - i);
	if (!remainder)
		return (NULL);
	i++;
	j = 0;
	while (saved[i])
		remainder[j++] = saved[i++];
	remainder[j] = '\0';
	free(saved);
	return (remainder);
}

static int	read_to_buffer(int fd, char **saved)
{
	char		*buffer;
	ssize_t		bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (-1);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr_gnl(*saved, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			return (-1);
		}
		buffer[bytes_read] = '\0';
		*saved = ft_strjoin_gnl(*saved, buffer);
	}
	free(buffer);
	return (0);
}

char	*get_next_line(int fd)
{
	static char	*saved;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (read_to_buffer(fd, &saved) == -1)
		return (NULL);
	line = extract_line(saved);
	saved = save_remainder(saved);
	return (line);
} 