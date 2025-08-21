#include "../include/minishell.h"

void	print_syntax_error(t_data *data)
{
	write(STDERR_FILENO, "syntax error near unexpected token\n", 36);
	data->i_exit = 2;
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
