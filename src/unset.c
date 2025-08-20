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
  len = ft_strlen(name);
  while (envp && envp[i])
  {
    if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
      return (i);
    i++;
  }
  return (-1);
}

static int	unset_env_var(char ***envp, const char *name)
{
  int		idx;
  int		count;
  int		i;
  char	**new_env;

  idx = find_env_index(*envp, name);
  if (idx < 0)
    return (0);
  count = 0;
  while ((*envp)[count])
    count++;
  new_env = malloc(sizeof(char *) * count);
  if (!new_env)
    return (1);
  i = -1;
  while (++i < idx)
    new_env[i] = (*envp)[i];
  while (i < count - 1)
  {
    new_env[i] = (*envp)[i + 1];
    i++;
  }
  new_env[i] = NULL;
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
      write(2, "unset: not a valid identifier\n", 31);
      ret = 1;
    }
    else if (!is_protected(argv[i]))
      unset_env_var(&data->envp, argv[i]);
    i++;
  }
  return (ret);
}
