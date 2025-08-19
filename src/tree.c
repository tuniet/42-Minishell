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
static int	handle_word(t_command *cmd, t_token *token)
{
	return (add_argument(cmd, token));
}

static int	handle_redirection(t_command *cmd, t_parse_ctx *ctx, int *i)
{
	if (*i + 1 > ctx->end || ctx->tokens[*i + 1]->type != TOKEN_WORD)
		return (0);
	if (!add_redirection(cmd, ctx->tokens[*i]->type,
			ctx->tokens[*i + 1]->content, ctx->data))
		return (0);
	(*i)++;
	return (1);
}

static int	process_tokens(t_command *cmd, t_parse_ctx *ctx)
{
	int	i;

	i = ctx->start;
	while (i <= ctx->end)
	{
		if (ctx->tokens[i]->type == TOKEN_WORD)
		{
			if (!handle_word(cmd, ctx->tokens[i]))
				return (0);
		}
		else if (is_redirection(ctx->tokens[i]->type))
		{
			if (!handle_redirection(cmd, ctx, &i))
				return (0);
		}
		else
			return (0);
		i++;
	}
	return (1);
}

t_treenode	*parse_simple_command(t_token *tokens[],
				int start, int end, t_data *data)
{
	t_treenode	*node;
	t_command	*cmd;
	t_parse_ctx	ctx;

	ctx.tokens = tokens;
	ctx.start = start;
	ctx.end = end;
	ctx.data = data;
	cmd = init_command();
	if (!cmd)
		return (NULL);
	if (!process_tokens(cmd, &ctx))
		return (free_command(cmd), NULL);
	if (!cmd->argv || !cmd->argv[0])
		return (free_command(cmd), NULL);
	node = new_node(TOKEN_COMMAND);
	if (!node)
		return (free_command(cmd), NULL);
	node->cmd = cmd;
	return (node);
}

t_treenode	*build_tree(t_token *tokens[], int start, int end, t_data *data)
{
	int	i;

	if (start > end)
		return (NULL);
	i = start;
	while (i <= end)
	{
		if (tokens[i]->type == TOKEN_AND || tokens[i]->type == TOKEN_OR)
			return (build_binary_node(tokens, start, end, i, data));
		i++;
	}
	i = start;
	while (i <= end)
	{
		if (tokens[i]->type == TOKEN_PIPE)
			return (build_binary_node(tokens, start, end, i, data));
		i++;
	}
	return (parse_simple_command(tokens, start, end, data));
}
