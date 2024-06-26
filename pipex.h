/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:01:26 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/23 14:53:33 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>

# include "ft_printf/libftprintf.h"
# include "libft/libft.h"

typedef struct s_data
{
	int		input;
	int		output;
	char	**arg;
	char	**paths;
	int		pipe[2];
	pid_t	pid1;
	pid_t	pid2;
}	t_data;

void	free_all(t_data *data, int error);
void	child_process1(t_data *data, char **argv, char **envp);
void	child_process2(t_data *data, char **argv, char **envp);
char	*find_path(char *command, char **paths, t_data *data);

#endif
