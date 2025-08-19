/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:19:22 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/19 23:09:55 by antoniof         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/minishell.h"

int	find_env_index(char **envp, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = strlen(name);
	while (envp && envp[i])
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	unset_env_var(char ***envp, const char *name)
{
    int	idx;
    int	count;
    int	i;

    idx = find_env_index(*envp, name);
    if (idx < 0)
        return (1);
    free((*envp)[idx]);
    (*envp)[idx] = NULL;
    count = 0;
    while ((*envp)[count])
        count++;
    for (i = idx; i < count; i++)
        (*envp)[i] = (*envp)[i + 1];
    return (0);
}

int	mini_unset(char **argv, t_data *data)
{
	if (!argv[1])
	{
		fprintf(stderr, "unset: se requiere VARIABLE\n");
		return (1);
	}
	if (unset_env_var(&data->envp, argv[1]) != 0)
	{
		fprintf(stderr, "unset: variable no encontrada\n");
		return (1);
	}
	return (0);
}
