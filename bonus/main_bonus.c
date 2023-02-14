/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:39:22 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/14 17:19:10 by mjourno          ###   ########.fr       */
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
	data->pid = malloc(sizeof(int) * argc - 3);
	if (!data->pid)
		free_all(data, 1);
	data->pipe = malloc(sizeof(int) * 2 * (argc - 4));
	if (!data->pipe)
		free_all(data, 1);
	int	i;
	i = 0;
	while (i < 2 * (argc - 4))
	{
		data->pipe[i] = -1;
		i++;
	}
	data->nb_cmd = argc - 3;
}

//verifie l'existence / acces aux fichiers
static void	verify_files(t_data *data, int argc, char **argv)
{
	int		i;
	char	garbarge[1];

	if (argv[1][0] == 'h' && argv[1][1] == 'e' && argv[1][2] == 'r'
		&& argv[1][3] == 'e' && argv[1][4] == '_' && argv[1][5] == 'd'
		&& argv[1][6] == 'o' && argv[1][7] == 'c' && argv[1][8] == '\0')
	{
		data->output = open(argv[argc - 1], O_CREAT | O_RDWR, 0644);
		if (data->output < 0)
		{
			free_all(data, 0);
			exit (ft_printf("%s: %s\n", strerror(errno), argv[argc - 1]));
		}
		i = 1;
		while (i == 1)
			i = read(data->output, garbarge, 1);
	}
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
	int		i;

	init_args(&data, argc);
	verify_files(&data, argc, argv);

	i = -1;
	while (++i < data.nb_cmd - 1)
		if (pipe(data.pipe + 2 * i) < 0)
			free_all(&data, 1);

	get_paths(envp, &data);

	child_process_input(&data, argv, envp);
	i = 0;
	while (++i < data.nb_cmd - 1)
		child_process(&data, argv, envp, i);
	child_process_output(&data, argv, envp, i);

	i = -1;
	while (++i < 2 * (data.nb_cmd - 1))
		close(data.pipe[i]);
	i = -1;
	while (++i < data.nb_cmd)
		wait(NULL);

	free_all(&data, 0);
	return (0);
}
