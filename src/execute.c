#include "../include/minishell.h"
/*

int execute_tree(t_treenode *node, char **envp);

int	apply_redirections(t_redirect *redir_list)
{
	t_redirect	*redir;
	int			fd;

	redir = redir_list;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == TOKEN_REDIRECT_OUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == TOKEN_APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			return (-1); // TODO: ADD HEREDOC

		if (fd < 0)
			return (-1);

		if (redir->type == TOKEN_REDIRECT_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

int	execute_command_node(t_treenode *node, char **envp)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**argv;

	pid = fork();
	if (pid == 0)
	{
		if (apply_redirections(node->cmd->redirects) != 0)
		{
			perror("Redirection error");
			exit(1);
		}
		//TODO: Expand $variables and $variables within quoted (") tokens in cmd->arg
		//Should we make a struct to know if the token is quoted      ????
		argv = expand(node->cmd->argv, node->cmd->argc, envp);
		if (!argv)
			return (NULL);
	
		//TODO: Add built-ins check here
		path = find_executable(node->cmd->argv[0], envp);
		fprintf(stderr, "CMD Path : %s\n\n", path);
		if (!path)
		{
			fprintf(stderr, "%s: command not found\n", node->cmd->argv[0]);
			exit(127);
		}
		execve(path, node->cmd->argv, envp);
		perror("execvp failed");
		exit(127);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
	{
		perror("fork failed");
		return (1);
	}
	return (WEXITSTATUS(status));
}

int	execute_pipe_node(t_treenode *node, char **envp)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		status_left;
	int		status_right;

	pipe(pipefd);
	left_pid = fork();
	if (left_pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(execute_tree(node->left, envp));
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(execute_tree(node->right, envp));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, &status_left, 0);
	waitpid(right_pid, &status_right, 0);
	return (WEXITSTATUS(status_right));
}

int	execute_logical_node(t_treenode *node, char **envp)
{
	int	left_status;

	left_status = execute_tree(node->left, envp);
	if (node->type == TOKEN_AND)
	{
		if (left_status == 0)
			return (execute_tree(node->right, envp));
		else
			return (left_status);
	}
	else if (node->type == TOKEN_OR)
	{
		if (left_status != 0)
			return (execute_tree(node->right, envp));
		else
			return (left_status);
	}
	return (1);
}

int execute_tree(t_treenode *node, char **envp)
{
	if (!node)
		return (0);
	if (node->type == TOKEN_COMMAND)
		return execute_command_node(node, envp);
	else if (node->type == TOKEN_PIPE)
		return execute_pipe_node(node, envp);
	else if (node->type == TOKEN_AND || node->type == TOKEN_OR)
		return execute_logical_node(node, envp);
	return (1);
}

*/
