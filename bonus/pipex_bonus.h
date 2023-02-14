/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:39:25 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/14 14:41:34 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <sys/wait.h>

# include "../ft_printf/libftprintf.h"
# include "../libft/libft.h"

typedef struct s_data
{
	int		input;
	int		output;
	char	**arg;
	char	**paths;
	int		*pipe;
	pid_t	*pid;
	int		nb_cmd;
}	t_data;

void	free_all(t_data *data, int error);
void	child_process_input(t_data *data, char **argv, char **envp);
void	child_process(t_data *data, char **argv, char **envp, int i);
void	child_process_output(t_data *data, char **argv, char **envp, int i);

#endif
