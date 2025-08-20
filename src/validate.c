/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:24:44 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 14:24:45 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (*str == '"' || *str == '\'')
		str++;
	if (!*str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	atollong(const char *str, long long *out)
{
	unsigned long long	val;
	int					sign;

	val = 0;
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		val = val * 10 + (*str - '0');
		if ((sign == 1 && val > LLONG_MAX) || (sign == -1
				&& val > (unsigned long long)LLONG_MAX + 1))
			return (0);
		str++;
	}
	*out = (long long)(val * sign);
	return (1);
}

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha((unsigned char)*s) && *s != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum((unsigned char)s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	is_protected(char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp(name, "PATH") == 0)
		return (1);
	if (ft_strcmp(name, "HOME") == 0)
		return (1);
	if (ft_strcmp(name, "PWD") == 0)
		return (1);
	if (ft_strcmp(name, "USER") == 0)
		return (1);
	return (0);
}
