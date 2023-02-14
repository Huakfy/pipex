/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:39:22 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/14 18:50:26 by mjourno          ###   ########.fr       */
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

//verifie l'existence / acces aux fichiers
static void	verify_files(t_data *data, int argc, char **argv)
{
	int		i;
	int		j;
	char	garbarge[1];

	if (ft_strncmp("!here_doc", argv[1], 9))
	{
		data->here_doc = 1;
		if (argc < 6)
		{
			free_all(data, 0);
			exit (ft_printf("Error\nMust have at least 5 arguments\n"));
		}
		data->input = open("here_doc.temp", O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (data->input < 0)
		{
			free_all(data, 0);
			exit (ft_printf("%s: %s\n", strerror(errno), "here_doc.temp"));
		}

		char	*tmp;
		data->limiter = ft_strjoin(argv[2], "\n");
		if (!data->limiter)
			free_all(data, 1);
		i = 0;
		while (argv[2][i])
		{
			data->limiter[i] = argv[2][i];
			i++;
		}
		data->limiter[i] = '\n';
		i++;
		data->limiter[i] = '\0';
		while (1)
		{
			write(1, "> ", 2);
			tmp = get_next_line(0);
			if (!tmp)
				free_all(data, 1);
			if (!ft_strncmp(tmp, data->limiter, ft_strlen(data->limiter)))
				break ;
			write(data->input, tmp, ft_strlen(tmp));
			free(tmp);
		}
		close(data->input);
		free(tmp);
		free(data->limiter);
		get_next_line(-1);
		open("here_doc.temp", O_RDONLY);

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
		//if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T' &&
		//	envp[i][3] == 'H' && envp[i][4] == '=')
		if (!ft_strncmp("PATH=",envp[i], 5))
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

	if (argc < 5)
		exit (ft_printf("Error\nMust have at least 4 arguments\n"));
	init_args(&data, argc);
	verify_files(&data, argc, argv);

	data.pid = malloc(sizeof(int) * argc - 3 - data.here_doc);
	if (!data.pid)
		free_all(&data, 1);
	data.pipe = malloc(sizeof(int) * (2 * (argc - 4) - data.here_doc));
	if (!data.pipe)
		free_all(&data, 1);
	i = 0;
	while (i < 2 * (argc - 4  - data.here_doc))
	{
		data.pipe[i] = -1;
		i++;
	}

	i = -1;
	while (++i < data.nb_cmd - 1 - data.here_doc)
		if (pipe(data.pipe + 2 * i) < 0)
			free_all(&data, 1);

	get_paths(envp, &data);

	child_process_input(&data, argv, envp);
	i = 0 + data.here_doc;
	while (++i < data.nb_cmd - 1)
		child_process(&data, argv, envp, i);
	child_process_output(&data, argv, envp, i);

	i = -1;
	while (++i < 2 * (data.nb_cmd - 1 - data.here_doc))
		close(data.pipe[i]);
	i = -1;
	while (++i < data.nb_cmd - data.here_doc)
		wait(NULL);

	free_all(&data, 0);
	return (0);
}
