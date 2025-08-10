#include "../include/minishell.h"

char	*expand_token(t_token *token, char **envp)
{
	char	*arg;
	int		i;
	char	*start;
	char	*end;

	if (tokens[i]->type == TOKEN_QUOTED || tokens[i]->type == TOKEN_VARIABLE)
	{
		start = *(token->content);

		return (arg);
	}
	return (strdup(token->content));
}

char	**expand(t_token **tokens, int argc, char **envp)
{
	char	**argv;
	int		i;
	int		j;
	
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	i = 0;
	j = 0;
	while (tokens[i])
	{
		argv[i] = expand_token(tokens[i], envp);
		if (!argv[i])
		{
			while (j < i)
				free(argv[j++]);
			free(argv);
			return (NULL);
		}
		i++;
	}
	argv[i] = NULL;
	return (argv);
}
