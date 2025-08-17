/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:18:58 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:18:59 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	add_argument(t_command *cmd, t_token *arg)
{
	t_token	**new_argv;
	size_t	new_capacity;

	if (cmd->argc + 1 >= cmd->capacity)
	{
		new_capacity = cmd->capacity * 2;
		new_argv = realloc(cmd->argv, sizeof(t_token *) * new_capacity);
		if (!new_argv)
			return (0);
		cmd->argv = new_argv;
		cmd->capacity = new_capacity;
	}
	cmd->argv[cmd->argc] = arg;
	cmd->argc++;
	cmd->argv[cmd->argc] = NULL;
	return (1);
}

static int	add_redirection(t_command *cmd, t_node_type type, char *filename)
{
	t_redirect	*redir;
	t_redirect	*tmp;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (0);
	redir->type = type;
	redir->filename = ft_strdup(filename);
	if (!redir->filename)
	{
		free(redir);
		return (0);
	}
	redir->next = NULL;
	if (!cmd->redirects)
		cmd->redirects = redir;
	else
	{
		tmp = cmd->redirects;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
	return (1);
}

static int	process_tokens(t_command *cmd, t_token *tokens[], int start,
		int end)
{
	int	i;

	i = start;
	while (i <= end)
	{
		if (tokens[i]->type == TOKEN_WORD)
		{
			if (!add_argument(cmd, tokens[i]))
				return (0);
		}
		else if (is_redirection(tokens[i]->type))
		{
			if (i + 1 > end || tokens[i + 1]->type != TOKEN_WORD)
				return (0);
			if (!add_redirection(cmd, tokens[i]->type, tokens[i + 1]->content))
				return (0);
			i++;
		}
		else
			return (0);
		i++;
	}
	return (1);
}

t_treenode	*parse_simple_command(t_token *tokens[], int start, int end)
{
	t_treenode	*node;
	t_command	*cmd;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	if (!process_tokens(cmd, tokens, start, end))
		return (free_command(cmd), NULL);
	if (!cmd->argv || !cmd->argv[0])
		return (free_command(cmd), NULL);
	node = new_node(TOKEN_COMMAND);
	if (!node)
		return (free_command(cmd), NULL);
	node->cmd = cmd;
	return (node);
}

t_treenode	*build_tree(t_token *tokens[], int start, int end)
{
	int	i;

	if (start > end)
		return (NULL);
	i = start;
	while (i <= end)
	{
		if (tokens[i]->type == TOKEN_AND || tokens[i]->type == TOKEN_OR)
			return (build_binary_node(tokens, start, end, i));
		i++;
	}
	i = start;
	while (i <= end)
	{
		if (tokens[i]->type == TOKEN_PIPE)
			return (build_binary_node(tokens, start, end, i));
		i++;
	}
	return (parse_simple_command(tokens, start, end));
}
