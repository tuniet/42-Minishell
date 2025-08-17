#include "../include/minishell.h"

int	argv_len(char **argv)
{
	int	i;
	
	i = 0;
	while (argv && argv[i])
		i++;
	return (i);
}

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
