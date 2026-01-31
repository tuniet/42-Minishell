/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:05:34 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/22 16:46:32 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char		*dup;
	size_t		len;
	size_t		i;

	len = 0;
	if (s == NULL)
		return (NULL);
	while (len < n && s[len] != '\0')
		len++;
	dup = malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	i = -1;
	while (++i < len)
	{
		dup[i] = s[i];
	}
	dup[len] = '\0';
	return (dup);
}
