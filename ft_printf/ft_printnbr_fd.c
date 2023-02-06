/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:27 by mjourno           #+#    #+#             */
/*   Updated: 2022/11/10 10:55:27 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static void	rec_putnbr_fd(unsigned int n, int fd, int *count)
{
	char	a;

	if (n >= 10)
		rec_putnbr_fd(n / 10, fd, count);
	a = n % 10 + '0';
	write(fd, &a, 1);
	*count += 1;
}

int	ft_printnbr_fd(int n, int fd)
{
	unsigned int	n_pos;
	int				count;

	count = 0;
	n_pos = n;
	if (n < 0)
	{
		write(fd, "-", 1);
		n_pos = n * -1;
		count++;
	}
	rec_putnbr_fd(n_pos, fd, &count);
	return (count);
}
