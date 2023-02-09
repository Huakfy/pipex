/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:32:42 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/09 16:14:49 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static int	print_correct_type(const char c, va_list args)
{
	if (c == 'c')
		return (ft_printchar_fd((unsigned char)va_arg(args, int), 2));
	if (c == 's')
		return (ft_printstr_fd(va_arg(args, char *), 2));
	if (c == 'p')
		return (ft_printmemory_fd(va_arg(args, long long unsigned int),
				"0123456789abcdef", 2));
	if (c == 'd' || c == 'i')
		return (ft_printnbr_fd(va_arg(args, int), 2));
	if (c == 'u')
		return (ft_printunbr_fd(va_arg(args, int), 2));
	if (c == 'x')
		return (ft_uibasefd(va_arg(args, unsigned int), "0123456789abcdef", 2));
	if (c == 'X')
		return (ft_uibasefd(va_arg(args, unsigned int), "0123456789ABCDEF", 2));
	if (c == '%')
		return (ft_printchar_fd('%', 2));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	int		count;
	int		i;
	va_list	args;

	va_start(args, str);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != '%')
			count += ft_printchar_fd(str[i], 2);
		else
		{
			count += print_correct_type(str[++i], args);
			if (!str[i])
				return (-1);
		}
		i++;
	}
	va_end(args);
	return (count);
}
