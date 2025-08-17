#include "../include/minishell.h"
#include <dirent.h>
#include <fnmatch.h>

char	**expand_wildcards(const char *pattern)
{
	DIR				*dir;
	struct dirent	*ent;
	char			**matches;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (ft_split(pattern, ' '));
	matches = NULL;
	count = 0;
	while ((ent = readdir(dir)))
	{
		if (fnmatch(pattern, ent->d_name, 0) == 0)
		{
			matches = realloc(matches, sizeof(char *) * (count + 2));
			matches[count++] = strdup(ent->d_name);
			matches[count] = NULL;
		}
	}
	closedir(dir);
	if (count == 0)
		return (ft_split(pattern, ' '));
	return (matches);
}


