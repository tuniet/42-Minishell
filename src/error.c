/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:32:32 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 14:32:33 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_echo_error(const char *filename, const char *arg)
{
	write(2, filename, ft_strlen(filename));
	write(2, ": ", 3);
	write(2, arg, ft_strlen(arg));
	write(2, "\n", 2);
}

void	print_export_error(const char *arg)
{
	write(2, "export: `", 10);
	write(2, arg, ft_strlen(arg));
	write(2, "': not a valid identifier\n", 27);
}

void	error_exit(const char *cmd, const char *msg, int i_exit)
{
	if (cmd)
	{
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, ": ", 3);
	}
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 2);
	exit(i_exit);
}

static void	check_file_permissions(const char *cmd)
{
	if (access(cmd, X_OK) != 0)
		error_exit(cmd, "Permission denied", 126);
}

static void	check_file_type(const char *cmd)
{
	struct stat	st;

	if (stat(cmd, &st) != 0)
		error_exit(cmd, "No such file or directory", 127);
	if ((st.st_mode & S_IFMT) == S_IFDIR)
		error_exit(cmd, "Is a directory", 126);
}

void	handle_exec_error_path(char *cmd, char *path)
{
	if (!path)
	{
		if (ft_strchr(cmd, '/'))
		{
			check_file_type(cmd);
			check_file_permissions(cmd);
			error_exit(cmd, "No such file or directory", 127);
		}
		error_exit(cmd, "command not found", 127);
	}
	check_file_type(path);
	check_file_permissions(path);
}
