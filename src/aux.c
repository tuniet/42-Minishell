#include "../include/minishell.h"

/*
void **ft_realloc (void **p)
{
	void *p;


	return (p);
}
*/

int is_metachar(int c)
{
	if (c == '\0' || c == '\t' || c == ' '
		|| c == '&' || c == '|'
		|| c == '<' || c == '>')
		return (1);
	return (0);
}

//TODO: Change the name of variable "char *line"
char	*get_token_end(char *line)
{
	char	quote;

	if (*line == '\0')
		return (line);
	if (*line == '|' || *line == '&' || *line == '<' || *line == '>')
	{
		if (*(line + 1) == *line)
			return (line + 2);
		return (line + 1);
	}
	while (*line && !is_metachar(*line))
	{
		if (*line == '\'' || *line == '"')
		{
			quote = *line++;
			while (*line && *line != quote)
				line++;
			if (*line == quote)
				line++;
		}
		else
			line++;
	}
	return (line);
}

