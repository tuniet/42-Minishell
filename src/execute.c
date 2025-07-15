#include "../include/minishell.h"
#include <sys/wait.h>

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

static char *find_executable(char *command, char **envp)
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

int get_redirection_fd(t_treenode *node)
{
	int		fd = -1;
	char	*filename;

	if (node->type == TOKEN_REDIRECT_IN)
		filename = node->right ? node->right->content : NULL;
	else if (node->type == TOKEN_REDIRECT_OUT)
		filename = node->left ? node->left->content : NULL;
	else
	{
		fprintf(stderr, "Unknown redirection type\n");
		return (-1);
	}

	if (!filename)
	{
		perror("Missing redirection target");
		return -1;
	}

	if (node->type == TOKEN_REDIRECT_IN)
		fd = open(filename, O_RDONLY);
	else if (node->type == TOKEN_REDIRECT_OUT)
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);

	if (fd < 0)
		perror("open");

	return fd;
}

t_treenode *setup_redirections(t_treenode *node)
{
	int fd;

	if (!node)
		return NULL;

	if (node->type == TOKEN_REDIRECT_IN || node->type == TOKEN_REDIRECT_OUT)
	{
		fd = get_redirection_fd(node);
		if (fd < 0)
			exit(1);

		if (node->type == TOKEN_REDIRECT_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);

		if (node->type == TOKEN_REDIRECT_IN)
			return setup_redirections(node->left);
		else
			return setup_redirections(node->right);
	}
	return node;
}


void execute_command_node(t_treenode *node, char **envp)
{
	char *content_aux;

	if (!node || !node->content)
		return;

	content_aux = strdup(node->content);
	if (!content_aux)
	{
		perror("strdup");
		exit(1);
	}

	char *argv[128];
	int i;
	char *token;

	i = 0;
	token = strtok(content_aux, " ");
	while (token && i < 127)
	{
		argv[i++] = token;
		token = strtok(NULL, " ");
	}
	argv[i] = NULL;

	if (i == 0)
	{
		free(content_aux);
		return;
	}

	char *cmd_path = find_executable(argv[0], envp);
	if (!cmd_path)
	{
		fprintf(stderr, "Command not found: %s\n", argv[0]);
		free(content_aux);
		exit(127);
	}

	execve(cmd_path, argv, envp);
	perror("execve");
	free(content_aux);
	exit(126);
}

void execute_pipe(t_treenode *node, char **envp)
{
	int pipefd[2];
	pid_t left_pid, right_pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}

	left_pid = fork();
	if (left_pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		execute_tree(node->left, envp);
		exit(0);
	}

	right_pid = fork();
	if (right_pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		execute_tree(node->right, envp);
		exit(0);
	}

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}

void execute_tree(t_treenode *node, char **envp)
{
	if (!node)
		return;

	if (node->type == TOKEN_PIPE)
	{
		execute_pipe(node, envp);
	}
	else if (node->type == TOKEN_REDIRECT_IN || node->type == TOKEN_REDIRECT_OUT)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			t_treenode *cmd = setup_redirections(node);
			execute_tree(cmd, envp);
			exit(0);
		}
		else
		{
			waitpid(pid, NULL, 0);
		}
	}
	else if (node->type == TOKEN_COMMAND)
	{
		pid_t pid = fork();
		if (pid == 0)
			execute_command_node(node, envp);
		else
			waitpid(pid, NULL, 0);
	}
}

