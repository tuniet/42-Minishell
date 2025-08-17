/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:04:43 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 21:04:44 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*build_env_var(const char *name, const char *value)
{
	size_t	len;
	char	*new_var;

	len = strlen(name) + strlen(value) + 2;
	new_var = malloc(len);
	if (!new_var)
		return (NULL);
	snprintf(new_var, len, "%s=%s", name, value);
	return (new_var);
}

static int	replace_env_var(char **envp, int idx, char *new_var)
{
	free(envp[idx]);
	envp[idx] = new_var;
	return (0);
}

static int	add_env_var(char ***envp, char *new_var)
{
	int		count;
	char	**new_envp;

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
	return (0);
}

static int	set_env_var(char ***envp, const char *name, const char *value)
{
	int		idx;
	char	*new_var;

	idx = find_env_index(*envp, name);
	new_var = build_env_var(name, value);
	if (!new_var)
		return (1);
	if (idx >= 0)
		return (replace_env_var(*envp, idx, new_var));
	else
		return (add_env_var(envp, new_var));
}

int	mini_export(char **argv, t_data *data)
{
	char		*eq;
	size_t		name_len;
	char		name[256];
	const char	*value;

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
	value = eq + 1;
	name_len = eq - argv[1];
	strncpy(name, argv[1], name_len);
	name[name_len] = '\0';
	if (set_env_var(&data->envp, name, value) != 0)
		return (perror("export"), 1);
	return (0);
}
