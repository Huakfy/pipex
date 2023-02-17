/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:22:54 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/17 16:03:40 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

//initialise tout les arguments a close / free
static void	init_args(t_data *data, int argc)
{
	data->input = -1;
	data->output = -1;
	data->arg = NULL;
	data->paths = NULL;
	data->pipe = NULL;
	data->pid = NULL;
	data->nb_cmd = argc - 3;
	data->here_doc = 0;
	data->limiter = NULL;
}

//verifie si un argument est vide
static void	verify_args(t_data *data, char **argv)
{
	int	i;
	int	j;

	i = -1;
	while (argv[++i])
	{
		j = 0;
		while (argv[i][j])
			j++;
		if (j == 0)
		{
			free_all(data, 0);
			exit (ft_printf("Error\nEmpty Argument\n"));
		}
	}
}

//verifie l'existence / acces aux fichiers
static void	verify_files(t_data *data, int argc, char **argv)
{
	verify_args(data, argv);
	if (!ft_strncmp("here_doc", argv[1], 9))
		here_doc(data, argc, argv);
	else
	{
		data->input = open(argv[1], O_RDONLY);
		if (data->input < 0)
			exit (ft_printf("%s: %s\n", strerror(errno), argv[1]));
		data->output = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (data->output < 0)
		{
			free_all(data, 0);
			exit (ft_printf("%s: %s\n", strerror(errno), argv[argc - 1]));
		}
	}
}

//initialise pid et pipe
static void	init_pid_pipe(t_data *data)
{
	int	i;

	data->pid = malloc(sizeof(int) * data->nb_cmd);
	if (!data->pid)
		free_all(data, 1);
	data->pipe = malloc(sizeof(int) * ((2 * data->nb_cmd) - 2));
	if (!data->pipe)
		free_all(data, 1);
	i = (2 * data->nb_cmd) - 2;
	while (--i >= 0)
		data->pipe[i] = -1;
	i = -1;
	while (++i < data->nb_cmd - 1)
		if (pipe(data->pipe + (2 * i)) < 0)
			free_all(data, 1);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	int		i;

	if (argc < 5)
		exit (ft_printf("Error\nMust have at least 4 arguments\n"));
	init_args(&data, argc);
	verify_files(&data, argc, argv);
	init_pid_pipe(&data);
	get_paths(envp, &data);
	launch_processes(&data, argc, argv, envp);
	close_pipes(&data);
	i = -1;
	while (++i < data.nb_cmd)
		wait(NULL);

	free_all(&data, 0);
	return (0);
}
