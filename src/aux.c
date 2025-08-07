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
	if (c == '\0' || c == '\t' || c == ' ' || c == '&'
		|| c == '|' || c == '\'' || c == '"'
		|| c == '<' || c == '>')
		return (1);
	return (0);
}

#define MAX_DELIMITER_SIZE 2
//TODO: Change the name of this please
//TODO: Change the name of variable "char *line"
char	*get_token_end(char *line)
{
	if (*line == '\0')
		return (line);
	if (*line == '|' || *line == '&' || *line == '<' || *line == '>')
	{
		if (*(line + 1) == *line)
			return (line + 2);
		return (line + 1);
	}
	if (*line == '\'' || *line == '"')
	{
		char quote = *line;
		line++;
		while (*line && *line != quote)
			line++;
		if (*line == quote)
			line++;
		return (line);
	}
	while (*line && *line != ' ' && *line != '\t' &&
		*line != '|' && *line != '&' && *line != '<' && *line != '>')
		line++;
	return (line);
}

