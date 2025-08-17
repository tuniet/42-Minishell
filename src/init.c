/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:17:43 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:17:45 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**cpy_env(char *envp[])
{
	char	**cpy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	cpy = malloc(sizeof(char *) * (i + 1));
	if (cpy == NULL)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		cpy[i] = ft_strdup(envp[i]);
		if (!cpy[i])
		{
			while (i >= 0)
				free(cpy[i--]);
			free(cpy);
			return (NULL);
		}
		i++;
	}
	cpy[i] = NULL;
	return (cpy);
}

void init_nulls(t_data* data)
{
	data->prompt = NULL;
	data->pwd = NULL;
	data->home = NULL;
	data->user = NULL;
	data->host = NULL;
	data->envp = NULL;
	data->tokens_size = 0;
	data->ast_root = NULL;
	data->iExit = 0;
}

int	init_data(t_data *data, char *envp[])
{
	init_nulls(data);
	if (envp == NULL || envp[0] == NULL || envp[0][0] == '\0')
		return(0);
	else
	{
		data->envp = cpy_env(envp);
		if (data->envp == NULL)
			return (0);
	}
	memset(data->tokens, 0x0, sizeof(data->tokens));
	data->pwd = getcwd(NULL, 0);
	return (1);
}
