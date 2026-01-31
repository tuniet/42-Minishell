/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:28:17 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/22 16:31:37 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_final_pattern(const char *p)
{
	while (*p == '*')
		p++;
	if (*p == 0)
		return (0);
	return (1);
}
