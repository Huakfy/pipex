/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 11:22:54 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/17 12:34:19 by mjourno          ###   ########.fr       */
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

//here_doc
static void	here_doc(t_data *data, int argc, char **argv)
{
	data->here_doc = 1;
	data->nb_cmd -= 1;
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

	//creer un limiter avec \n pour gnl
	char	*tmp;
	data->limiter = ft_strjoin(argv[2], "\n");
	if (!data->limiter)
		free_all(data, 1);

	//lit jusqu'a trouver le limiter
	while (1)
	{
		if (write(1, "> ", 2) < 0)
			free_all(data, 1);
		tmp = get_next_line(0);
		if (!tmp)
			free_all(data, 1);
		if (!ft_strncmp(tmp, data->limiter, ft_strlen(data->limiter)))
			break ;
		if (write(data->input, tmp, ft_strlen(tmp)) < 0)
			free_all(data, 1);
		free(tmp);
	}
	free(tmp);
	free(data->limiter);
	data->limiter = NULL;
	get_next_line(-1);
	if (close(data->input) < 0)
		free_all(data, 1);

	//open data->output
	open("here_doc.temp", O_RDONLY);
	data->output = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
	if (data->output < 0)
	{
		free_all(data, 0);
		exit (ft_printf("%s: %s\n", strerror(errno), argv[argc - 1]));
	}
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

//recupere la liste des paths depuis envp
static void	get_paths(char **envp, t_data *data)
{
	int		i;

	i = 0;
	while (envp[i])
	{
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

	//creation pid
	data.pid = malloc(sizeof(int) * data.nb_cmd);
	if (!data.pid)
		free_all(&data, 1);

	//creation pipe
	data.pipe = malloc(sizeof(int) * ((2 * data.nb_cmd) - 2));
	if (!data.pipe)
		free_all(&data, 1);
	//initialisation pipe a -1
	i = (2 * data.nb_cmd) - 2;
	while (--i >= 0)
		data.pipe[i] = -1;
	//initialisation pipe
	i = -1;
	while (++i < data.nb_cmd - 1)
		if (pipe(data.pipe + (2 * i)) < 0)
			free_all(&data, 1);

	//Cherche PATHS dans l'environnement
	get_paths(envp, &data);

	data.index_pipe = (2 * data.nb_cmd) - 3;
	data.index_pid = 0;
	child_process_input(&data, argv[2 + data.here_doc], envp);
	data.index_pipe -= 2;
	data.index_pid += 1;

	i = 2 + data.here_doc;
	while (++i < argc - 2)
	{
		child_process(&data, argv[i], envp);
		data.index_pipe -= 2;
		data.index_pid += 1;
	}

	child_process_output(&data, argv[argc - 2], envp);

	close_pipes(&data);
	i = -1;
	while (++i < data.nb_cmd)
		wait(NULL);

	free_all(&data, 0);
	return (0);
}
