/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:01:26 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/10 12:10:09 by mjourno          ###   ########.fr       */
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
	int		error;
}	t_data;

void	free_all(t_data *data, int error);
void	child_process1(t_data *data, char **argv, char **envp);
void	child_process2(t_data *data, char **argv, char **envp);

#endif
