#include "../include/minishell.h"

int	is_numeric(const char *str)
{
	int i;

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
		if (!ft_isdigit((unsigned char)str[i]))
			return (0);
		i++;
	}
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

int is_protected(char *name)
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
