#include "../include/minishell.h"

void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

int	argv_len(char **argv)
{
	int	i;
	
	i = 0;
	while (argv && argv[i])
		i++;
	return (i);
}

char	**copy_argv(char **src, int *index, char **res)
{
	int	i;

	i = 0;
	while (src && src[i])
	{
		res[*index] = strdup(src[i]);
		if (!res[*index])
		{
			free_argv(res);
			return (NULL);
		}
		(*index)++;
		i++;
	}
	return (res);
}

char	**argv_join(char **argv, char **exp)
{
	int		size1;
	int		size2;
	int		i;
	char	**res;

	size1 = argv_len(argv);
	size2 = argv_len(exp);
	res = malloc(sizeof(char *) * (size1 + size2 + 1));
	if (!res)
		return (NULL);

	i = 0;
	if (!copy_argv(argv, &i, res))
		return (NULL);
	if (!copy_argv(exp, &i, res))
		return (NULL);

	res[i] = NULL;
	free_argv(argv);
	free_argv(exp);
	return (res);
}
