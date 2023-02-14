/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 10:22:05 by mjourno           #+#    #+#             */
/*   Updated: 2022/11/18 10:22:05 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		++i;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	while ((s[i]))
	{
		if (s[i] == (unsigned char)c)
			return ((char *)s + i);
		++i;
	}
	if (s[i] == (unsigned char)c)
		return ((char *)s + i);
	return (NULL);
}

char	*ft_strjoin2(char *s1, char *s2, int lenS2, int lenS1)
{
	int		k;
	char	*result;

	if (!s1)
	{
		s1 = malloc(1 * sizeof(char));
		s1[0] = '\0';
	}
	if (!s1 || !s2)
		return (NULL);
	result = malloc(sizeof(char) * (lenS1 + lenS2 + 1));
	if (!result)
		return (NULL);
	k = -1;
	while (++k < lenS1)
		result[k] = s1[k];
	--k;
	while (++k < lenS1 + lenS2)
		result[k] = s2[k - lenS1];
	result[k] = '\0';
	free(s1);
	return (result);
}

char	*ft_strdup_but_better(char *src, int len)
{
	int		i;
	char	*dest;

	if (!len)
		return (NULL);
	dest = malloc(sizeof(char) * (len + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (src[i] && i < len)
	{
		dest[i] = src[i];
		++i;
	}
	dest[i] = '\0';
	return (dest);
}
