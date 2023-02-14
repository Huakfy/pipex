/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 14:51:32 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/14 18:24:25 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*read_endofline(int fd, char *str)
{
	char	*buffer;
	int		len;
	int		len_total;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	len = 1;
	len_total = 0;
	if (str)
		len_total = ft_strlen(str);
	while (!ft_strchr(str, '\n') && len)
	{
		len = read(fd, buffer, BUFFER_SIZE);
		if (len == -1)
		{
			free (buffer);
			return (NULL);
		}
		buffer[len] = '\0';
		str = ft_strjoin2(str, buffer, len, len_total);
		len_total += len;
	}
	free(buffer);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		*line;
	char		*next_line;
	int			i;

	if (fd < 0 && str)
		free(str);
	if (fd < 0 || (BUFFER_SIZE <= 0 && !str))
		return (NULL);
	str = read_endofline(fd, str);
	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
		++i;
	if (str[i] == '\n')
		++i;
	line = ft_strdup_but_better(str, i);
	next_line = ft_strdup_but_better(str + i, ft_strlen(str + i));
	free(str);
	str = next_line;
	if (!str)
		free(str);
	return (line);
}
