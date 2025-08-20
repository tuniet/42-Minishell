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
  int		i;
  char	*eq;

  if (!argv[1])
    return (0);
  i = 1;
  while (argv[i])
  {
    if (!is_valid_identifier(argv[i]))
    {
      print_export_error(argv[i]);
      return (1);
    }
    eq = ft_strchr(argv[i], '=');
    if (eq)
    {
      *eq = '\0';
      set_env_var(&data->envp, argv[i], eq + 1);
    }
    i++;
  }
  return (0);
}
