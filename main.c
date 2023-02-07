/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:01:35 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/07 14:50:58 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_paths(char **envp)
{
	char	*path;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T' && envp[i][3] == 'H' && envp[i][4] == '=')
		{
			path = ft_strdup(envp[i] + 5);
			if (!path)
				exit(ft_printf("%s\n",strerror(errno)));
		}

		i++;
	}
	return (path);
}

char	*free_paths_exit(char **paths, int error)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	if (error)
		exit(ft_printf("%s\n",strerror(errno)));
	return (NULL);
}

char	*find_path(char *command, char **paths)
{
	int		i;
	char	*temp;
	char	*temp2;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			free_paths_exit(paths, 1);
		temp2 = ft_strjoin(temp, command);
		free(temp);
		if (!temp2)
			free_paths_exit(paths, 1);
		if (!access(temp2, F_OK))
		{
			if (!access(temp2, X_OK))
				return (temp2);
			exit(ft_printf("%s: %s\n",strerror(errno), temp2));
		}
		free(temp2);
		i++;
	}
	return (free_paths_exit(paths, 1));
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc != 5)
		exit (ft_printf("Error\nMust have 4 arguments\n"));

	data.input = open(argv[1], O_RDONLY);
	data.output = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (data.input < 0 || data.output < 0)
		exit (ft_printf("%s: %s\n",strerror(errno), argv[1]));

	char	**split_cmd;
	split_cmd = ft_split(argv[2], ' ');
	if (!split_cmd)
		exit(ft_printf("%s\n",strerror(errno)));
	data.cmd = ft_strdup(split_cmd[0]);
	if (!data.cmd)
		exit(ft_printf("%s\n",strerror(errno)));

	data.arg = split_cmd;

	char	*paths;
	paths = get_paths(envp);

	char	**split_paths;
	split_paths = ft_split(paths, ':');
	if (!split_paths)
		exit(ft_printf("%s\n",strerror(errno)));
	free(paths);

	data.path = find_path(data.cmd, split_paths);
	free_paths_exit(split_paths, 0);

	free(data.arg[0]);
	data.arg[0] = data.path;

	ft_printf(argv[1]);
	//char	*newargv[3] = {data.path, argv[1], NULL};
	//execve(data.path, data.arg, NULL);

	free_paths_exit(data.arg, 0);
	free(data.cmd);
	return (0);
}
