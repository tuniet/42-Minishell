/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:16:42 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:16:43 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void free_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return;
	while (tokens[i])
	{
		free(tokens[i]->content);
		tokens[i]->content = NULL;
		free(tokens[i]);
		tokens[i] = NULL;
		i++;
	}
}

void free_tree(t_treenode *node)
{
	if (!node)
		return;
	free_tree(node->left);
	free_tree(node->right);
	if (node->type == TOKEN_COMMAND)
		free_command(node->cmd);
	free(node);
}

static void free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		envp[i] = NULL;
		i++;
	}
	free(envp);
}

void free_all(t_data *data, int flag)
{
	if (!data)
		return ;
	if (flag)
	{
		free_envp(data->envp);
		data->envp = NULL;
		free(data->pwd);
		data->pwd = NULL;
	}
	free_tree(data->ast_root);
	data->ast_root = NULL;
	free_tokens(data->tokens);
	memset(data->tokens, 0x0, sizeof(data->tokens));
	data->tokens_size = 0;
}
