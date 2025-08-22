/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:10:31 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/22 19:42:51 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static pid_t	exec_pipe_left(t_treenode *node, int *pipefd, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execute_tree(node, data->envp, data));
	}
	return (pid);
}

static pid_t	exec_pipe_right(t_treenode *node, int *pipefd, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execute_tree(node, data->envp, data));
	}
	return (pid);
}

int	execute_pipe_node(t_treenode *node, t_data *data)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;
	int		status_right;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid_left = exec_pipe_left(node->left, pipefd, data);
	pid_right = exec_pipe_right(node->right, pipefd, data);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	setup_signals();
	data->i_exit = WEXITSTATUS(status_right);
	return (data->i_exit);
}

int	execute_tree(t_treenode *node, char **envp, t_data *data)
{
	if (!node)
		return (1);
	if (node->type == TOKEN_COMMAND)
		return (execute_command_node(node, envp, data));
	else if (node->type == TOKEN_PIPE)
		return (execute_pipe_node(node, data));
	else if (node->type == TOKEN_AND || node->type == TOKEN_OR)
		return (execute_logical_node(node, envp, data));
	return (0);
}
