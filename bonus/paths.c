/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 15:36:03 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/27 13:20:13 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

//recupere la liste des paths depuis envp
void	get_paths(char **envp, t_data *data)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp("PATH=", envp[i], 5))
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

//Y a plus de place en dessous
static char	*norm(t_data *data)
{
	free_all(data, 0);
	exit (ft_printf("Error\nEmpty Argument\n"));
	return (NULL);
}

//Si / dans la commande chercher directement ce programme
static char	*absolute_path(char *command, t_data *data)
{
	char	*temp;

	if (!access(command, F_OK))
	{
		if (!access(command, X_OK))
		{
			temp = ft_strdup(command);
			if (!temp)
				free_all(data, 1);
			return (temp);
		}
		ft_printf("%s: %s\n", strerror(errno), command);
		free_all(data, 0);
		exit (1);
	}
	ft_printf("Command not found: %s\n", command);
	free_all(data, 0);
	exit (1);
	return (NULL);
}

//Trouve le path ou ce situe la commande
static char	*find_path(char *command, char **paths, t_data *data)
{
	int		i;
	char	*temp;
	char	*temp2;

	i = -1;
	while (paths[++i] && command)
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
			ft_error(temp2, data);
		}
		free(temp2);
	}
	return (norm(data));
}

char	*find_path0(t_data *data, char *cmd)
{
	data->arg = ft_split(cmd, ' ');
	if (!data->arg)
		free_all(data, 1);
	if (ft_strchr(data->arg[0], '/'))
		return (absolute_path(data->arg[0], data));
	return (find_path(data->arg[0], data->paths, data));
}
