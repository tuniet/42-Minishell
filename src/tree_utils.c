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

void	print_tree(t_treenode *node, int level)
{
	t_redirect	*redir;
	int			i;

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
	{
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
		redir = node->cmd->redirects;
		while (redir)
		{
			i = 0;
			while (i < level + 1)
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
