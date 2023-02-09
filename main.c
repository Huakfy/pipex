/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:01:35 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/09 16:55:22 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//initialise tout les arguments a close / free
void	init_args(t_data *data)
{
	data->input = -1;
	data->output = -1;
	data->arg =NULL;
	data->paths = NULL;
	data->pipe[0] = -1;
	data->pipe[1] = -1;
	data->error = 0;
}

//close / free tout les arguments en fin de programme / cas d'erreur
void	free_all(t_data *data, int error)
{
	int	i;

	if (data->input && data->input != -1)
		close (data->input);
	if (data->output && data->output != -1)
		close (data->output);
	if (data->arg)
	{
		i = -1;
		while (data->arg[++i])
			free(data->arg[i]);
		free(data->arg);
	}
	if (data->paths)
	{
		i = -1;
		while (data->paths[++i])
			free(data->paths[i]);
		free(data->paths);
	}
	if (data->pipe[0] && data->pipe[0] != -1)
		close(data->pipe[0]);
	if (data->pipe[1] && data->pipe[1] != -1)
		close(data->pipe[1]);
	if (!data->pid1 || !data->pid2)
		data->error = 1;
	if (error)
		exit(ft_printf("%s\n",strerror(errno)));
}

//recupere la liste des paths depuis envp
void	get_paths(char **envp, t_data *data)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T' &&
			envp[i][3] == 'H' && envp[i][4] == '=')
			{
				data->paths = ft_split(envp[i] + 5, ':');
				if (!data->paths)
					free_all(data, 1);
				return ;
			}
		i++;
	}
	ft_printf("No PATH");
	free_all(data, 1);
}

//Trouve le path ou ce situe la commande
char	*find_path(char *command, char **paths, t_data *data)
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

void	verify_files(t_data *data, int argc, char **argv)
{
	data->input = open(argv[1], O_RDONLY);
	if (data->input < 0)
		exit (ft_printf("%s: %s\n",strerror(errno), argv[1]));
	data->output = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (data->output < 0)
	{
		free_all(data, 0);
		exit (ft_printf("%s: %s\n",strerror(errno), argv[argc - 1]));
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init_args(&data);
	if (argc != 5)
		exit (ft_printf("Error\nMust have 4 arguments\n"));
	if (pipe(data.pipe) < 0)
		free_all(&data, 1);
	get_paths(envp, &data);
	child_process1(&data, argv, envp);
	if (data.error == 1)
		free_all(&data, 1);
	child_process2(&data, argv, envp);
	if (data.error == 1)
		free_all(&data, 1);
	if (!data.pid1)
		waitpid(data.pid1, NULL, 0);
	if (!data.pid2)
		waitpid(data.pid2, NULL, 0);
	free_all(&data, 0);
	return (0);
}
