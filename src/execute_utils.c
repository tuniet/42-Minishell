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

static char	*join_path(char *dir, char *command)
{
	size_t	len1;
	size_t	len2;
	char	*full;

	len1 = ft_strlen(dir);
	len2 = ft_strlen(command);
	full = malloc(len1 + len2 + 2);
	if (!full)
		return (NULL);
	ft_strcpy(full, dir);
	full[len1] = '/';
	ft_strcpy(full + len1 + 1, command);
	return (full);
}

static char	*check_direct_path(char *command)
{
	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	return (NULL);
}

static char	*search_in_path(char *paths, char *command)
{
	char	*dir;
	char	*full_path;
	char	*saveptr;

	saveptr = paths;
	dir = next_token(&saveptr, ':');
	while (dir)
	{
		if (*dir == '\0')
			dir = ".";
		full_path = join_path(dir, command);
		if (!full_path)
			break ;
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		dir = next_token(&saveptr, ':');
	}
	free(paths);
	return (NULL);
}

char	*find_executable(char *command, char **envp)
{
	char	*path;
	char	*paths;

	if (ft_strchr(command, '/'))
		return (check_direct_path(command));
	path = mini_getenv("PATH", envp);
	if (!path)
		return (NULL);
	paths = ft_strdup(path);
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
