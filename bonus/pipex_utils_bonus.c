/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:39:28 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/14 16:25:02 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	free_array(char **array)
{
	int	i;

	if (array)
	{
		i = -1;
		while (array[++i])
			free(array[i]);
		free(array);
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
	free(data->pipe);
	free(data->pid);
	if (error)
		exit(error);
}

//Trouve le path ou ce situe la commande
static char	*find_path(char *command, char **paths, t_data *data)
{
	int		i;
	char	*temp;
	char	*temp2;

	i = -1;
	while (paths[++i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			free_all(data, 1);
		temp2 = ft_strjoin(temp, command);
		free(temp);
		if (!temp2)
			free_all(data, 1);
		if (!access(temp2, F_OK))
		{
			if (!access(temp2, X_OK))
				return (temp2);
			free_all(data, 0);
			exit (ft_printf("%s: %s\n", strerror(errno), temp2));
		}
		free(temp2);
	}
	free_all(data, 1);
	return (NULL);
}

void	child_process_input(t_data *data, char **argv, char **envp)
{
	char	*path;
	int		i;

	data->pid[0] = fork();
	if (data->pid[0] < 0)
		free_all(data, 1);
	if (!data->pid[0])
	{
		if (dup2(data->input, STDIN_FILENO) < 0)
			free_all(data, 1);
		if (dup2(data->pipe[(data->nb_cmd - 2) * 2 + 1], STDOUT_FILENO) < 0)
			free_all(data, 1);
		i = 2 * (data->nb_cmd - 1);
		while (--i >= 0)
			close(data->pipe[i]);
		close(data->input);
		data->arg = ft_split(argv[2], ' ');
		if (!data->arg)
			free_all(data, 1);
		path = find_path(data->arg[0], data->paths, data);
		if (execve(path, data->arg, envp) == -1)
		{
			free(path);
			free_all(data, 1);
		}
		free(path);
	}
}

void	child_process(t_data *data, char **argv, char **envp, int i)
{
	char	*path;
	int		j;

	data->pid[i] = fork();
	if (data->pid[i] < 0)
		free_all(data, 1);
	if (!data->pid[i])
	{
		if (dup2(data->pipe[(data->nb_cmd - 2) * 2 + 1 - (i * 2) + 1], STDIN_FILENO) < 0)
			free_all(data, 1);
		if (dup2(data->pipe[(data->nb_cmd - 2) * 2 + 1 - (i * 2)], STDOUT_FILENO) < 0)
			free_all(data, 1);
		j = 2 * (data->nb_cmd - 1);
		while (--j >= 0)
			close(data->pipe[j]);
		data->arg = ft_split(argv[i + 2], ' ');
		if (!data->arg)
			free_all(data, 1);
		path = find_path(data->arg[0], data->paths, data);
		if (execve(path, data->arg, envp) == -1)
		{
			free(path);
			free_all(data, 1);
		}
		free(path);
	}
}

void	child_process_output(t_data *data, char **argv, char **envp, int i)
{
	char	*path;
	int		j;

	data->pid[i] = fork();
	if (data->pid[i] < 0)
		free_all(data, 1);
	if (!data->pid[i])
	{
		if (dup2(data->pipe[0], STDIN_FILENO) < 0)
			free_all(data, 1);
		if (dup2(data->output, STDOUT_FILENO) < 0)
			free_all(data, 1);
		j = 2 * (data->nb_cmd - 1);
		while (--j >= 0)
			close(data->pipe[j]);
		close(data->output);
		data->arg = ft_split(argv[i + 2], ' ');
		if (!data->arg)
			free_all(data, 1);
		path = find_path(data->arg[0], data->paths, data);
		if (execve(path, data->arg, envp) == -1)
		{
			free(path);
			free_all(data, 1);
		}
		free(path);
	}
}
