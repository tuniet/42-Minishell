/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:25:59 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/22 16:49:38 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
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

int	is_redirection(t_node_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

t_treenode	*new_node(t_node_type type)
{
	t_treenode	*node;

	node = malloc(sizeof(t_treenode));
	if (!node)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->cmd = NULL;
	return (node);
}

t_treenode	*build_binary_node(t_token *tokens[], int start_end[],
		int op_index, t_data *data)
{
	t_treenode	*node;
	t_treenode	*left;
	t_treenode	*right;
	int			se[2];

	se[0] = start_end[0];
	se[1] = op_index - 1;
	node = new_node(tokens[op_index]->type);
	if (!node)
		return (NULL);
	left = build_tree(tokens, se, data);
	if (!left)
		return (free(node), NULL);
	se[0] = op_index + 1;
	se[1] = start_end[1];
	right = build_tree(tokens, se, data);
	if (!right)
	{
		free_tree(left);
		return (free(node), NULL);
	}
	node->left = left;
	node->right = right;
	return (node);
}
