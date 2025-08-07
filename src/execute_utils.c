#include "../include/minishell.h"

static char *join_path(const char *dir, const char *command)
{
	size_t	len1;
	size_t	len2;
	char	*full;

	len1 = strlen(dir);
	len2 = strlen(command);
	full = malloc(len1 + len2 + 2);
	if (!full)
		return (NULL);

	strcpy(full, dir);
	full[len1] = '/';
	strcpy(full + len1 + 1, command);
	return (full);
}

char *find_executable(char *command, char **envp)
{
	char	*path;
	char	*paths, *dir, *full_path;
	char	*saveptr;

	path = mini_getenv("PATH", envp);
	if (strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (strdup(command));
		else
			return (NULL);
	}
	if (!path)
		return (NULL);

	paths = strdup(path);
	if (!paths)
		return (NULL);

	dir = strtok_r(paths, ":", &saveptr);
	while (dir)
	{
		full_path = join_path(dir, command);
		if (!full_path)
			break;

		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		dir = strtok_r(NULL, ":", &saveptr);
	}
	free(paths);
	return (NULL);
}

