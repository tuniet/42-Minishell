#include "../include/minishell.h"

int	update_envp(char **envp, char *var, char *new_value)
{
  int	i;
  size_t var_len;
  size_t new_value_len;
  char *new_entry;

  i = 0;
  var_len = ft_strlen(var);
  new_value_len = ft_strlen(new_value);
  new_entry = malloc(var_len + 1 + new_value_len + 1);
  if (!new_entry)
    return (0);
  ft_strcpy(new_entry, var);
  new_entry[var_len] = '=';
  ft_strcpy(new_entry + var_len + 1, new_value);
  while (envp[i])
  {
    if (ft_strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
    {
      free(envp[i]);
      envp[i] = NULL;
      envp[i] = new_entry;
      return (1);
    }
    i++;
  }
  envp[i] = new_entry;
  return (1);
}
