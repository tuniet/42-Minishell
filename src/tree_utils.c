/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:19:07 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:19:09 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_command *init_command(void)
{
	t_command	*cmd;

	cmd = calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argc = 0;
	cmd->capacity = 10;
	cmd->argv = malloc(sizeof(t_token) * cmd->capacity);
	if (!cmd->argv)
	{
		free(cmd);
		return (NULL);
	}
	cmd->argv[0] = NULL;
	return (cmd);
}

static void	print_redirects(t_redirect *redir, int level)
{
	int	i;

	while (redir)
	{
		i = 0;
		while (i < level)
		{
			printf("    ");
			i++;
		}
		printf("Redirect: %s %s\n",
			redir->type == TOKEN_REDIRECT_IN ? "<" :
			redir->type == TOKEN_REDIRECT_OUT ? ">" :
			redir->type == TOKEN_HEREDOC ? "<<" :
			redir->type == TOKEN_APPEND ? ">>" : "?",
			redir->filename);
		redir = redir->next;
	}
}

static void	print_command(t_treenode *node, int level)
{
	int	i;

	printf("[COMMAND]");
	if (node->cmd && node->cmd->argv)
	{
		i = 0;
		while (node->cmd->argv[i])
		{
			printf(" %s", node->cmd->argv[i]->content);
			i++;
		}
	}
	printf("\n");
	if (node->cmd)
		print_redirects(node->cmd->redirects, level + 1);
}

void	print_tree(t_treenode *node, int level)
{
	int	i;

	if (!node)
		return;
	print_tree(node->right, level + 1);
	i = 0;
	while (i < level)
	{
		printf("    ");
		i++;
	}
	if (node->type == TOKEN_PIPE)
		printf("[PIPE]\n");
	else if (node->type == TOKEN_AND)
		printf("[AND]\n");
	else if (node->type == TOKEN_OR)
		printf("[OR]\n");
	else if (node->type == TOKEN_COMMAND)
		print_command(node, level);
	print_tree(node->left, level + 1);
}


int is_redirection(t_node_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT ||
	type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

t_treenode *new_node(t_node_type type)
{
	t_treenode	*node;

	node = malloc(sizeof(t_treenode));
	if (!node)
		return (NULL); // TODO: handle error properly
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	return (node);
}
