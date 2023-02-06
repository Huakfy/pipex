/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printunbr_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:27 by mjourno           #+#    #+#             */
/*   Updated: 2022/11/10 10:55:27 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static void	rec_putunbr_fd(unsigned int n, int fd, int *count)
{
	char	a;

	if (n >= 10)
		rec_putunbr_fd(n / 10, fd, count);
	a = n % 10 + '0';
	write(fd, &a, 1);
	*count += 1;
}

int	ft_printunbr_fd(unsigned int n, int fd)
{
	int	count;

	count = 0;
	rec_putunbr_fd(n, fd, &count);
	return (count);
}
