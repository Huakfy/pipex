/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:01:35 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/07 11:01:42 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	verify_access(char *from, char *to)
{
	int	error;

	error = 0;
	if (access(from, F_OK) == -1 || access(from, R_OK) == -1)
		error = ft_printf("%s: %s\n",strerror(errno), from);
	if (access(to, F_OK) == -1 ||access(to, R_OK) == -1)
		error = ft_printf("%s: %s\n",strerror(errno), to);
	if (error)
		exit (error);
}

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

char	*free_paths_exit(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
	exit(ft_printf("%s\n",strerror(errno)));
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
			free_paths_exit(paths);
		temp2 = ft_strjoin(temp, command);
		free(temp);
		if (!temp2)
			free_paths_exit(paths);
		if (!access(temp2, F_OK))
		{
			if (!access(temp2, X_OK))
				return (temp2);
			exit(ft_printf("%s: %s\n",strerror(errno), temp2));
		}
		free(temp2);
		i++;
	}
	return (free_paths_exit(paths));
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
		exit (ft_printf("Error\nMust have 4 arguments\n"));
	verify_access(argv[1], argv[argc - 1]);

	char	*paths;
	paths = get_paths(envp);

	char	**split_paths;
	split_paths = ft_split(paths, ':');
	if (!split_paths)
		exit(ft_printf("%s\n",strerror(errno)));
	free(paths);

	char	*path;
	path = find_path(argv[2], split_paths);
	int	i = 0;
	while (split_paths[i])
	{
		free(split_paths[i]);
		i++;
	}
	free(split_paths);

	char	*newargv[3] = {path, argv[1], NULL};
	execve(path, newargv, NULL);
	//ft_printf("%d\n", execve("/bin/cat", newargv, NULL));
	free(path);
	return (0);
}
