/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 16:05:34 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 16:05:35 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strndup(const char *s, size_t n)
{
    if (s == NULL) {
        return NULL;
    } 
    size_t len = 0;
    while (len < n && s[len] != '\0') {
        len++;
    }    
    char *dup = malloc(len + 1);
    if (dup == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        dup[i] = s[i];
    }
    dup[len] = '\0';
    
    return dup;
}