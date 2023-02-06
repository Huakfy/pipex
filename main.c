/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:01:35 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/06 15:17:27 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	verify_access(char *from, char *to)
{
	int	error;

	error = 0;
	if (access(from, 0) == -1)
		error = ft_printf("%s: %s\n",strerror(errno), from);
	if (access(to, 0) == -1)
		error = ft_printf("%s: %s\n",strerror(errno), to);
	if (error)
		exit (error);
}

int	main(int argc, char **argv)
{
	char	*file;

	if (argc != 5)
		exit (ft_printf("Error\nMust have 4 arguments\n"));
	verify_access(argv[1], argv[argc - 1]);
	file = 	read_file(argv[1]);
	free(file);
	return (0);
}
