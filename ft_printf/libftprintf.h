/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 11:39:37 by mjourno           #+#    #+#             */
/*   Updated: 2022/11/10 11:39:37 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <unistd.h>
# include <stdarg.h>

int	ft_printf(const char *str, ...);
int	ft_printunbr_fd(unsigned int n, int fd);
int	ft_uibasefd(unsigned int nbr, char *base, int fd);
int	ft_printchar_fd(char c, int fd);
int	ft_printstr_fd(char *s, int fd);
int	ft_printnbr_fd(int n, int fd);
int	ft_printmemory_fd(long long unsigned int n, char *base, int fd);

#endif
