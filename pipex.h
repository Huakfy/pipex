/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:01:26 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/06 14:51:06 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>

# include "ft_printf/libftprintf.h"

char	*read_file(char *file);

#endif
