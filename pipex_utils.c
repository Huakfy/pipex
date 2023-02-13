/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 12:07:03 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/10 14:25:48 by mjourno          ###   ########.fr       */
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
	}
}

//close / free tout les arguments en fin de programme / cas d'erreur
void	free_all(t_data *data, int error)
{
	char *err;

	err = NULL;
	if (!data->pid1)
	{
		err = strerror(errno);
		waitpid(data->pid1, NULL, 0);
	}
	else if (!data->pid2)
	{
		err = strerror(errno);
		waitpid(data->pid2, NULL, 0);
	}
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
	if (err)
		exit (ft_printf("%s\n",err));
	if (error)
		exit(ft_printf("%s\n",strerror(errno)));
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
			exit (ft_printf("%s: %s\n",strerror(errno), temp2));
		}
		free(temp2);
	}
	free_all(data, 1);
	return (NULL);
}

void	child_process1(t_data *data, char **argv, char **envp)
{
	char *path;

	data->pid1 = fork();
	if (data->pid1 < 0)
		free_all(data, 1);
	if (!data->pid1)
	{
		if (dup2(data->input, STDIN_FILENO) < 0)
			free_all(data, 1);
		if (dup2(data->pipe[1], STDOUT_FILENO) < 0)
			free_all(data, 1);
		close(data->pipe[0]);
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

void	child_process2(t_data *data, char **argv, char **envp)
{
	data->pid2 = fork();
	if (data->pid2 < 0)
		free_all(data, 1);
	if (!data->pid2)
	{
		if (dup2(data->output, STDOUT_FILENO) < 0)
			free_all(data, 1);
		if (dup2(data->pipe[0], STDIN_FILENO) < 0)
			free_all(data, 1);
		close(data->pipe[1]);
		close(data->output);
		data->arg = ft_split(argv[3], ' ');
		if (!data->arg)
			free_all(data, 1);
		char *path;
		path = find_path(data->arg[0], data->paths, data);
		if (execve(path, data->arg, envp) == -1)
		{
			free(path);
			free_all(data, 1);
		}
		free(path);
	}
}
