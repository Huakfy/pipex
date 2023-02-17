/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:39:28 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/17 16:02:51 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_pipes(t_data *data)
{
	int	i;

	i = (2 * data->nb_cmd) - 2;
	while (--i >= 0)
		if (close(data->pipe[i]) < 0)
			free_all(data, 1);
}

static void	child_process_input(t_data *data, char *cmd, char **envp)
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

static void	child_process(t_data *data, char *cmd, char **envp)
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

static void	child_process_output(t_data *data, char *cmd, char **envp)
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

void	launch_processes(t_data *data, int argc, char **argv, char **envp)
{
	int	i;

	data->index_pipe = (2 * data->nb_cmd) - 3;
	data->index_pid = 0;
	child_process_input(data, argv[2 + data->here_doc], envp);
	data->index_pipe -= 2;
	data->index_pid += 1;
	i = 2 + data->here_doc;
	while (++i < argc - 2)
	{
		child_process(data, argv[i], envp);
		data->index_pipe -= 2;
		data->index_pid += 1;
	}
	child_process_output(data, argv[argc - 2], envp);
}
