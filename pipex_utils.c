/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 12:07:03 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/23 14:36:48 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	free_array(char **array)
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
	if (error)
		ft_printf("%s\n", strerror(errno));
	if (data->input && data->input != -1)
		close (data->input);
	if (data->output && data->output != -1)
		close (data->output);
	free_array(data->arg);
	free_array(data->paths);
	if (data->pipe[0] && data->pipe[0] != -1)
		close(data->pipe[0]);
	if (data->pipe[1] && data->pipe[1] != -1)
		close(data->pipe[1]);
	if (error)
		exit(error);
}

void	child_process1(t_data *data, char **argv, char **envp)
{
	char	*path;

	data->arg = ft_split(argv[2], ' ');
	if (!data->arg)
		free_all(data, 1);
	path = find_path(data->arg[0], data->paths, data);
	data->pid1 = fork();
	if (data->pid1 < 0)
		free_all(data, 1);
	if (!data->pid1)
	{
		if (dup2(data->input, STDIN_FILENO) < 0
			|| dup2(data->pipe[1], STDOUT_FILENO) < 0)
			free_all(data, 1);
		if (close(data->pipe[0]) < 0 || close(data->input) < 0)
			free_all(data, 1);
		if (execve(path, data->arg, envp) == -1)
		{
			free(path);
			free_all(data, 1);
		}
	}
	free(path);
	free_array(data->arg);
}

void	child_process2(t_data *data, char **argv, char **envp)
{
	char	*path;

	data->arg = ft_split(argv[3], ' ');
	if (!data->arg)
		free_all(data, 1);
	path = find_path(data->arg[0], data->paths, data);
	data->pid2 = fork();
	if (data->pid2 < 0)
		free_all(data, 1);
	if (!data->pid2)
	{
		if (dup2(data->output, STDOUT_FILENO) < 0
			|| dup2(data->pipe[0], STDIN_FILENO) < 0)
			free_all(data, 1);
		if (close(data->pipe[1]) < 0 || close(data->output) < 0)
			free_all(data, 1);
		if (execve(path, data->arg, envp) == -1)
		{
			free(path);
			free_all(data, 1);
		}
	}
	free(path);
}
