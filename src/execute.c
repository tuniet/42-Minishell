/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:16:04 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:16:05 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	open_redir(t_redirect *redir, t_data *data)
{
	int		fd;
	char	*filename;

	filename = expand_token_(redir->filename, data->envp, data->iExit);
	if (!filename)
		return (-1);
	if (redir->type == TOKEN_REDIRECT_IN)
		fd = open(filename, O_RDONLY);
	else if (redir->type == TOKEN_REDIRECT_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = heredoc(filename, data);
	free(filename);
	return (fd);
}

int	apply_redirections(t_redirect *redir_list, t_data *data)
{
	t_redirect	*redir;
	int			fd;

	redir = redir_list;
	while (redir)
	{
		fd = open_redir(redir, data);
		if (fd < 0)
			return (-1);
		if (redir->type == TOKEN_REDIRECT_IN || redir->type == TOKEN_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

static void	run_child_process(t_treenode *node,
				char **argv, char **envp, t_data *data)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(node->cmd->redirects, data) != 0)
	{
		perror("Redirection error");
		exit(1);
	}
	if (!argv)
		exit(1);
	path = find_executable(argv[0], envp);
	if (!path)
	{
		fprintf(stderr, "%s: command not found\n", argv[0]);
		exit(127);
	}
	execve(path, argv, envp);
	perror("execv failed");
	exit(127);
}

static int	handle_child_status(pid_t pid, t_data *data)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	data->iExit = WEXITSTATUS(status);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
	data->iExit = WEXITSTATUS(status);
	return (data->iExit);
}

int	execute_command_node(t_treenode *node, char **envp, t_data *data)
{
	pid_t	pid;
	char	**argv;

	argv = expand(node->cmd->argv, envp, data->iExit);
	if (is_builtin(argv[0]))
		return (execute_builtin(argv, data));
	pid = fork();
	if (pid == 0)
		run_child_process(node, argv, envp, data);
	free_argv(argv);
	return (handle_child_status(pid, data));
}
