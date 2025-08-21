/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:29:19 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/21 19:26:04 by antoniof         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/minishell.h"

static int	open_redir(t_redirect *redir, t_data *data)
{
	int		fd;
	char	*filename;

	if (redir->type == TOKEN_HEREDOC)
		return (dup(redir->hered_fd));
	filename = expand_token_(redir->filename, data->envp, data->i_exit);
	if (!filename)
		return (-1);
	if (redir->type == TOKEN_REDIRECT_IN || redir->type == TOKEN_HEREDOC)
		fd = open(filename, O_RDONLY);
	else if (redir->type == TOKEN_REDIRECT_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = -1;
	free(filename);
	return (fd);
}

int	apply_redirections(t_redirect *redir_list, t_data *data)
{
	t_redirect	*redir;
	int			fd;

	/*
		aux = redir_list;
		while (aux)
		{
			printf("filename %s\n", aux->filename);
			aux = aux->next;
		}
	*/
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

int	apply_echo_redirections(t_redirect *redir_list, t_data *data)
{
	t_redirect	*redir;
	int			fd;

	/*
		aux = redir_list;
		while (aux)
		{
			printf("filename %s\n", aux->filename);
			aux = aux->next;
		}
	*/
	redir = redir_list;
	while (redir)
	{
		fd = open_redir(redir, data);
		if (fd < 0)
			return (print_echo_error(redir->filename, strerror(errno)), -1);
		if (redir->type == TOKEN_REDIRECT_OUT || redir->type == TOKEN_APPEND)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

static void	run_child_process(t_treenode *node, char **argv, char **envp,
		t_data *data)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(node->cmd->redirects, data) != 0)
		error_exit("Redirection error", strerror(errno), 1);
	if (!argv || !argv[0])
		exit(0);
	path = find_executable(argv[0], envp);
	handle_exec_error_path(argv[0], path);
	execve(path, argv, envp);
	perror("execve");
	exit(126);
}

static int	handle_child_status(pid_t pid, t_data *data)
{
	int	status;
	int	sig;

	ignore_signals();
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
		{
			printf("\n");
			data->i_exit = 130;
		}
		else if (sig == SIGQUIT)
		{
			printf("Quit: (Core dumped)\n");
			data->i_exit = 131;
		}
		else
			data->i_exit = 128 + sig;
	}
	else if (WIFEXITED(status))
		data->i_exit = WEXITSTATUS(status);
	return (data->i_exit);
}

int	execute_command_node(t_treenode *node, char **envp, t_data *data)
{
	pid_t	pid;
	char	**argv;

	argv = expand(node->cmd->argv, envp, data->i_exit);
	/*
		i = 0;
		while (argv[i])
		{
			printf("expanded [%d] - %s\n", i+1, argv[i]);
			i++;
		}
	*/
	if (is_builtin(argv[0]))
		return (execute_builtin(node, argv, data));
	pid = fork();
	if (pid == 0)
		run_child_process(node, argv, envp, data);
	free_argv(argv);
	return (handle_child_status(pid, data));
}
