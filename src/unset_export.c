/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:19:22 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:19:25 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	find_env_index(char **envp, const char *name)
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

static int	set_env_var(char ***envp, const char *name, const char *value)
{
	int		idx;
	size_t	len;
	char	*new_var;
	int		count;
	char	**new_envp;

	idx = find_env_index(*envp, name);
	len = strlen(name) + strlen(value) + 2;
	new_var = malloc(len);
	if (!new_var)
		return (1);
	snprintf(new_var, len, "%s=%s", name, value);
	if (idx >= 0)
	{
		free((*envp)[idx]);
		(*envp)[idx] = new_var;
	}
	else
	{
		count = 0;
		while ((*envp)[count])
			count++;
		new_envp = realloc(*envp, sizeof(char *) * (count + 2));
		if (!new_envp)
		{
			free(new_var);
			return (1);
		}
		new_envp[count] = new_var;
		new_envp[count + 1] = NULL;
		*envp = new_envp;
	}
	return (0);
}

static int	unset_env_var(char ***envp, const char *name)
{
	int	idx;
	int	count;

	idx = find_env_index(*envp, name);
	if (idx < 0)
		return (1);
	free((*envp)[idx]);
	count = 0;
	while ((*envp)[count])
		count++;
	for (int i = idx; i < count - 1; i++)
		(*envp)[i] = (*envp)[i + 1];
	(*envp)[count - 1] = NULL;
	return (0);
}

int	mini_export(char **argv, t_data *data)
{
	char		*eq;
	size_t		name_len;
	char		name[256];
	const char	*value = eq + 1;

	if (!argv[1])
	{
		write(2, "export: se requiere VARIABLE=VALOR\n", 35);
		return (1);
	}
	eq = strchr(argv[1], '=');
	if (!eq || eq == argv[1])
	{
		write(2, "export: formato incorrecto, use VARIABLE=VALOR\n", 45);
		return (1);
	}
	name_len = eq - argv[1];
	strncpy(name, argv[1], name_len);
	name[name_len] = '\0';
	if (set_env_var(&data->envp, name, value) != 0)
	{
		perror("export");
		return (1);
	}
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
