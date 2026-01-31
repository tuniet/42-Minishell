/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:25:42 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 14:25:47 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	find_env_index(char **envp, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static void	copy_env_without_index(char **new_env, char **old_env, int skip_idx)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (old_env[i])
	{
		if (i != skip_idx)
		{
			new_env[j] = old_env[i];
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
}

static int	unset_env_var(char ***envp, const char *name)
{
	int		idx;
	int		count;
	char	**new_env;

	idx = find_env_index(*envp, name);
	if (idx < 0)
		return (0);
	count = count_env_vars(*envp);
	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return (1);
	copy_env_without_index(new_env, *envp, idx);
	free((*envp)[idx]);
	free(*envp);
	*envp = new_env;
	return (0);
}

int	mini_unset(char **argv, t_data *data)
{
	int	i;
	int	ret;

	ret = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			write(STDERR_FILENO, "unset: not a valid identifier\n", 31);
			ret = 0;
		}
		else if (!is_protected(argv[i]))
			unset_env_var(&data->envp, argv[i]);
		i++;
	}
	return (ret);
}
