/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:01:35 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/23 14:37:25 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//initialise tout les arguments a close / free
static void	init_args(t_data *data)
{
	data->input = -1;
	data->output = -1;
	data->arg = NULL;
	data->paths = NULL;
	data->pipe[0] = -1;
	data->pipe[1] = -1;
	data->pid1 = -1;
	data->pid2 = -1;
}

//verifie l'existence / acces aux fichiers
static void	verify_files(t_data *data, int argc, char **argv)
{
	int	i;
	int	j;

	data->input = open(argv[1], O_RDONLY);
	if (data->input < 0)
		exit (ft_printf("%s: %s\n", strerror(errno), argv[1]));
	data->output = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (data->output < 0)
	{
		free_all(data, 0);
		exit (ft_printf("%s: %s\n", strerror(errno), argv[argc - 1]));
	}
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

//recupere la liste des paths depuis envp
static void	get_paths(char **envp, t_data *data)
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

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	init_args(&data);
	if (argc != 5)
		exit (ft_printf("Error\nMust have 4 arguments\n"));
	if (pipe(data.pipe) < 0)
		free_all(&data, 1);
	verify_files(&data, argc, argv);
	get_paths(envp, &data);
	child_process1(&data, argv, envp);
	child_process2(&data, argv, envp);
	close(data.pipe[0]);
	close(data.pipe[1]);
	wait(NULL);
	wait(NULL);
	free_all(&data, 0);
	return (0);
}
