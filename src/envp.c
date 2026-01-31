/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:32:02 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 14:32:04 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*create_env_entry(char *var, char *new_value)
{
	size_t	var_len;
	size_t	new_value_len;
	char	*new_entry;

	var_len = ft_strlen(var);
	new_value_len = ft_strlen(new_value);
	new_entry = malloc(var_len + 1 + new_value_len + 1);
	if (!new_entry)
		return (NULL);
	ft_strcpy(new_entry, var);
	new_entry[var_len] = '=';
	ft_strcpy(new_entry + var_len + 1, new_value);
	return (new_entry);
}

int	update_envp(char **envp, char *var, char *new_value)
{
	int		i;
	size_t	var_len;
	char	*new_entry;

	new_entry = create_env_entry(var, new_value);
	if (!new_entry)
		return (0);
	i = 0;
	var_len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
		{
			free(envp[i]);
			envp[i] = new_entry;
			return (1);
		}
		i++;
	}
	envp[i] = new_entry;
	return (1);
}
