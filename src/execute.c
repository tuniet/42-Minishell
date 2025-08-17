#include "../include/minishell.h"

int execute_tree(t_treenode *node, char **envp, t_data *data);

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

#define IS_BUILTIN 0
int	execute_command_node(t_treenode *node, char **envp, t_data *data)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**argv;
	//TODO: free argv 
	argv = expand(node->cmd->argv, envp, data->iExit);
	printf("\nAFTER EXPANSION : \n");
	for (int i = 0; argv[i]; i++)
	{
		printf("Expanded [%d] : %s\n", i+1, argv[i]);
	}
	//TODO: update iExit
	if (is_builtin(argv[0]))
        return execute_builtin(argv, data);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (apply_redirections(node->cmd->redirects) != 0)
		{
			perror("Redirection error");
			exit(1);
		}
		if (!argv)
			exit(1);
		path = find_executable(argv[0], envp);
		fprintf(stderr, "CMD Path : %s\n\n", path);
		if (!path)
		{
			fprintf(stderr, "%s: command not found\n", argv[0]);
			exit(127);
		}
		execve(path, argv, envp);
		perror("execv failed");
		exit(127);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	data->iExit = WEXITSTATUS(status);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);

	//TODO: Comprobar si el resultado de $? es correcto al salir por Ctrl+C
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
    	write(1, "\n", 1);
	return (data->iExit);
	/*
	waitpid(pid, &status, 0);
	data->iExit = WEXITSTATUS(status);
	return (data->iExit);
	*/
}

int	execute_pipe_node(t_treenode *node, char **envp, t_data *data)
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
		exit(execute_tree(node->left, envp, data));
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(execute_tree(node->right, envp, data));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left_pid, &status_left, 0);
	waitpid(right_pid, &status_right, 0);
	data->iExit = WEXITSTATUS(status_right);
	return (data->iExit);
}

int	execute_logical_node(t_treenode *node, char **envp, t_data *data)
{
	int	left_status;

	left_status = execute_tree(node->left, envp, data);
	if (node->type == TOKEN_AND)
	{
		if (left_status == 0)
			data->iExit = execute_tree(node->right, envp, data);
		else
			data->iExit = left_status;
	}
	else if (node->type == TOKEN_OR)
	{
		if (left_status != 0)
			data->iExit = execute_tree(node->right, envp, data);
		else
			data->iExit = left_status;
	}
	return (data->iExit);
}

int	execute_tree(t_treenode *node, char **envp, t_data *data)
{
	if (!node)
		return (1);
	if (node->type == TOKEN_COMMAND)
		return execute_command_node(node, envp, data);
	else if (node->type == TOKEN_PIPE)
		return execute_pipe_node(node, envp, data);
	else if (node->type == TOKEN_AND || node->type == TOKEN_OR)
		return execute_logical_node(node, envp, data);
	return (0);
}
