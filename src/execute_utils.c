/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:16:11 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:16:12 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*join_path(const char *dir, const char *command)
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

static char	*check_direct_path(char *command)
{
	if (access(command, X_OK) == 0)
		return (strdup(command));
	return (NULL);
}

static char	*search_in_path(char *paths, char *command)
{
	char	*dir;
	char	*full_path;
	char	*saveptr;

	dir = strtok_r(paths, ":", &saveptr);
	while (dir)
	{
		full_path = join_path(dir, command);
		if (!full_path)
			break ;
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

char	*find_executable(char *command, char **envp)
{
	char	*path;
	char	*paths;

	path = mini_getenv("PATH", envp);
	if (strchr(command, '/'))
		return (check_direct_path(command));
	if (!path)
		return (NULL);
	paths = strdup(path);
	if (!paths)
		return (NULL);
	return (search_in_path(paths, command));
}

int	execute_logical_node(t_treenode *node, char **envp, t_data *data)
{
	int	left_status;

	left_status = execute_tree(node->left, envp, data);
	if (node->type == TOKEN_AND)
	{
		if (left_status == 0)
			data->i_exit = execute_tree(node->right, envp, data);
		else
			data->i_exit = left_status;
	}
	else if (node->type == TOKEN_OR)
	{
		if (left_status != 0)
			data->i_exit = execute_tree(node->right, envp, data);
		else
			data->i_exit = left_status;
	}
	return (data->i_exit);
}
