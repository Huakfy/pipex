/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 14:50:04 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/06 15:19:15 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static int	len_file(char *file)
{
	int		i;
	int		fd;
	int		reading;
	char	bin[1];

	fd = open(file, O_RDONLY);
	if (fd == -1)
		exit(ft_printf("%s: %s\n",strerror(errno), file));
	i = 0;
	reading = 1;
	while (reading > 0)
	{
		reading = read(fd, bin, 1);
		i++;
	}
	if (reading == -1)
		exit(ft_printf("%s: %s\n",strerror(errno), file));
	if (close(fd) == -1)
		exit(ft_printf("%s: %s\n",strerror(errno), file));
	return (i);
}

char	*read_file(char *file)
{
	int		fd;
	int		len;
	char	*result;

	len = len_file(file);
	if (!len)
		return (NULL);
	fd = open(file, O_RDONLY);
	if (fd == -1)
		exit(ft_printf("%s: %s\n",strerror(errno), file));
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		exit(ft_printf("%s\n",strerror(errno)));
	if (read(fd, result, len) == -1)
		exit(ft_printf("%s: %s\n",strerror(errno), file));
	result[len] = '\0';
	if (close(fd) == -1)
	{
		free(result);
		exit(ft_printf("%s\n",strerror(errno)));
	}
	return (result);
}
