/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 15:34:01 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/27 13:20:17 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_array(char **array)
{
	int	i;

	if (array)
	{
		i = -1;
		while (array[++i])
			free(array[i]);
		free(array);
		array = NULL;
	}
}

//close / free tout les arguments en fin de programme / cas d'erreur
void	free_all(t_data *data, int error)
{
	int	i;

	if (error)
		ft_printf("%s\n", strerror(errno));
	if (data->input && data->input != -1)
		close (data->input);
	if (data->output && data->output != -1)
		close (data->output);
	free_array(data->arg);
	free_array(data->paths);
	if (data->pid)
		free(data->pid);
	if (data->here_doc && access("../here_doc.temp", F_OK))
		unlink("here_doc.temp");
	if (data->limiter)
		free(data->limiter);
	i = -1;
	if (data->pipe)
		while (++i < (2 * data->nb_cmd) - 2)
			close(data->pipe[i]);
	if (data->pipe)
		free(data->pipe);
	if (error)
		exit(error);
}

//y'a plus de place dans find_path
void	ft_error(char *temp2, t_data *data)
{
	ft_printf("%s: %s\n", strerror(errno), temp2);
	free_all(data, 0);
	free(temp2);
	exit (1);
}
