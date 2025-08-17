/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:18:37 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:18:39 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!dest)
		return (NULL);
	while (dest[i] != '\0')
		i++;
	if (src)
	{
		while (src[j] != '\0')
		{
			dest[i + j] = src[j];
			j++;
		}
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_strdup(const char *src)
{
	size_t	len;
	char	*dup;

	len = ft_strlen(src);
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, src, len);
	dup[len] = '\0';
	return (dup);
}
size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
		i++;
	}
	return (dest);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	for (i = 0; i < size - 1 && src[i] != '\0'; i++)
	{
		dst[i] = src[i];
	}
	if (size > 0)
	{
		dst[i] = '\0';
	}
	return (ft_strlen(src));
}
