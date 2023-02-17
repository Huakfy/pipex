/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 15:39:21 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/17 16:14:56 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

//lit l'entrée utilisateur et le stock dans le fichier
static void	read_input(t_data *data, char **argv)
{
	char	*tmp;

	data->limiter = ft_strjoin(argv[2], "\n");
	if (!data->limiter)
		free_all(data, 1);
	while (1)
	{
		if (write(1, "> ", 2) < 0)
			free_all(data, 1);
		tmp = get_next_line(0);
		if (!tmp)
			free_all(data, 1);
		if (!ft_strncmp(tmp, data->limiter, ft_strlen(data->limiter)))
			break ;
		if (write(data->input, tmp, ft_strlen(tmp)) < 0)
			free_all(data, 1);
		free(tmp);
	}
	free(tmp);
	free(data->limiter);
	data->limiter = NULL;
	get_next_line(-1);
	if (close(data->input) < 0)
		free_all(data, 1);
}

//here_doc
void	here_doc(t_data *data, int argc, char **argv)
{
	data->here_doc = 1;
	data->nb_cmd -= 1;
	if (argc < 6)
	{
		free_all(data, 0);
		exit (ft_printf("Error\nMust have at least 5 arguments\n"));
	}
	data->input = open("here_doc.temp", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (data->input < 0)
	{
		free_all(data, 0);
		exit (ft_printf("%s: %s\n", strerror(errno), "here_doc.temp"));
	}
	read_input(data, argv);
	data->input = open("here_doc.temp", O_RDONLY);
	if (data->input < 0)
		free_all(data, 1);
	data->output = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
	if (data->output < 0)
	{
		free_all(data, 0);
		exit (ft_printf("%s: %s\n", strerror(errno), argv[argc - 1]));
	}
}
