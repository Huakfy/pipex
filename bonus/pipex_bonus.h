/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 11:39:25 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/27 11:56:39 by mjourno          ###   ########.fr       */
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
# include "../gnl/get_next_line.h"

typedef struct s_data
{
	int		input;
	int		output;
	char	**arg;
	char	**paths;
	int		*pipe;
	int		index_pipe;
	pid_t	*pid;
	int		index_pid;
	int		nb_cmd;
	int		here_doc;
	char	*limiter;
}	t_data;

void	free_all(t_data *data, int error);
void	launch_processes(t_data *data, int argc, char **argv, char **envp);
void	close_pipes(t_data *data);
void	free_array(char **array);
char	*find_path0(t_data *data, char *cmd);
void	get_paths(char **envp, t_data *data);
void	here_doc(t_data *data, int argc, char **argv);
void	ft_error(char *temp2, t_data *data);

#endif
