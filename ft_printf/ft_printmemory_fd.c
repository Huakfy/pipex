/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printmemory_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 16:46:37 by mjourno           #+#    #+#             */
/*   Updated: 2022/11/17 14:05:20 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static unsigned int	ft_verify_base(char *base)
{
	int	i;
	int	j;

	i = -1;
	while (base[++i])
	{
		j = i;
		while (base[++j])
			if (base[i] == base[j])
				return (0);
		if (base[i] == '+' || base[i] == '-')
			return (0);
	}
	if (i <= 1)
		return (0);
	return (i);
}

void	rec_putmemory_fd(long long unsigned int n, char *base, int fd,
		int *count)
{
	unsigned int	len_base;

	len_base = ft_verify_base(base);
	if (n >= len_base)
		rec_putmemory_fd(n / len_base, base, fd, count);
	ft_printchar_fd(base[n % len_base], fd);
	*count += 1;
}

int	ft_printmemory_fd(long long unsigned int n, char *base, int fd)
{
	unsigned int	len_base;
	int				count;

	if (n == 0)
	{
		ft_printstr_fd("(nil)", 1);
		return (5);
	}
	ft_printstr_fd("0x", 1);
	count = 2;
	len_base = ft_verify_base(base);
	if (len_base == 0)
		return (0);
	rec_putmemory_fd(n, base, fd, &count);
	return (count);
}
