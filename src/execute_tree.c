/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:10:31 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 20:10:33 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	exec_pipe_side(t_treenode *node, int in_fd, int out_fd,
		t_data *data)
{
	if (fork() == 0)
	{
		if (in_fd >= 0)
			dup2(in_fd, STDIN_FILENO);
		if (out_fd >= 0)
			dup2(out_fd, STDOUT_FILENO);
		close(in_fd);
		close(out_fd);
		exit(execute_tree(node, data->envp, data));
	}
}

int	execute_pipe_node(t_treenode *node, t_data *data)
{
	int		pipefd[2];
	int		status_left;
	int		status_right;

	pipe(pipefd);
	exec_pipe_side(node->left, -1, pipefd[1], data);
	exec_pipe_side(node->right, pipefd[0], -1, data);
	close(pipefd[0]);
	close(pipefd[1]);
	wait(&status_left);
	wait(&status_right);
	data->iExit = WEXITSTATUS(status_right);
	return (data->iExit);
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
