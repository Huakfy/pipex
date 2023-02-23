/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 14:36:34 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/23 14:56:16 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//Y a plus de place dans find_path (utils)
static char	*norm(t_data *data)
{
	free_all(data, 0);
	exit (ft_printf("Error\nEmpty/Wrong Argument\n"));
	return (NULL);
}

static void	norm2(t_data *data, char *temp2)
{
	ft_printf("%s: %s\n", strerror(errno), temp2);
	free_all(data, 0);
	free(temp2);
	exit (1);
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
char	*find_path(char *command, char **paths, t_data *data)
{
	int		i;
	char	*temp;
	char	*temp2;

	if (ft_strchr(command, '/'))
		return (absolute_path(command, data));
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
			norm2(data, temp2);
		}
		free(temp2);
	}
	return (norm(data));
}
