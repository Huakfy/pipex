/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:14:22 by mjourno           #+#    #+#             */
/*   Updated: 2022/11/10 10:14:22 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	int		k;
	char	*result;

	i = 0;
	while (s1[i])
		i++;
	j = 0;
	while (s2[j])
		j++;
	result = malloc(sizeof(char) * (i + j + 1));
	if (!result)
		return (NULL);
	k = -1;
	while (++k < i)
		result[k] = s1[k];
	k--;
	while (++k < i + j)
		result[k] = s2[k - i];
	result[k] = '\0';
	return (result);
}
