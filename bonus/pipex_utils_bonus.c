/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:39:28 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/17 13:04:25 by mjourno          ###   ########.fr       */
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
	if(data->pid)
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

//Trouve le path ou ce situe la commande
static char	*find_path(char *command, char **paths, t_data *data)
{
	int		i;
	char	*temp;
	char	*temp2;

	i = -1;
	while (paths[++i] && command)
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
	free_all(data, 0);
	exit (ft_printf("Error\nEmpty Argument\n"));
	return (NULL);
}

void	close_pipes(t_data *data)
{
	int	i;

	i = (2 * data->nb_cmd) - 2;
	while (--i >= 0)
		if (close(data->pipe[i]) < 0)
			free_all(data, 1);
}

static char	*find_path0(t_data *data, char *cmd)
{
	data->arg = ft_split(cmd, ' ');
	if (!data->arg)
		free_all(data, 1);
	return (find_path(data->arg[0], data->paths, data));
}

void	child_process_input(t_data *data, char *cmd, char **envp)
{
	char	*path;

	path = find_path0(data, cmd);

	data->pid[data->index_pid] = fork();
	if (data->pid[data->index_pid] < 0)
		free_all(data, 1);
	if (!data->pid[data->index_pid])
	{
		if (dup2(data->input, STDIN_FILENO) < 0)
			free_all(data, 1);
		if (dup2(data->pipe[data->index_pipe], STDOUT_FILENO) < 0)
			free_all(data, 1);

		close_pipes(data);
		if (close(data->input) < 0)
			free_all(data, 1);

		if (execve(path, data->arg, envp) == -1)
			free_all(data, 1);
	}
	free(path);
	free_array(data->arg);
}

void	child_process(t_data *data, char *cmd, char **envp)
{
	char	*path;

	path = find_path0(data, cmd);

	data->pid[data->index_pid] = fork();
	if (data->pid[data->index_pid] < 0)
		free_all(data, 1);
	if (!data->pid[data->index_pid])
	{
		if (dup2(data->pipe[data->index_pipe + 1], STDIN_FILENO) < 0)
			free_all(data, 1);
		if (dup2(data->pipe[data->index_pipe], STDOUT_FILENO) < 0)
			free_all(data, 1);

		close_pipes(data);

		if (execve(path, data->arg, envp) == -1)
			free_all(data, 1);
	}
	free(path);
	free_array(data->arg);
}

void	child_process_output(t_data *data, char *cmd, char **envp)
{
	char	*path;

	path = find_path0(data, cmd);

	data->pid[data->index_pid] = fork();
	if (data->pid[data->index_pid] < 0)
		free_all(data, 1);
	if (!data->pid[data->index_pid])
	{
		if (dup2(data->pipe[0], STDIN_FILENO) < 0)
			free_all(data, 1);
		if (dup2(data->output, STDOUT_FILENO) < 0)
			free_all(data, 1);

		close_pipes(data);
		if (close(data->output) < 0)
			free_all(data, 1);

		if (execve(path, data->arg, envp) == -1)
			free_all(data, 1);
	}
	free(path);
}
