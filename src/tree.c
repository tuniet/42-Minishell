#include "../include/minishell.h"

static int	add_argument(t_command *cmd, char *arg)
{
	int		count;
	char	**new_argv;

	count = 0;
	while (cmd->argv[count])
		count++;

	new_argv = realloc(cmd->argv, sizeof(char *) * (count + 2));
	if (!new_argv)
		return (-1);

	new_argv[count] = ft_strdup(arg);
	new_argv[count + 1] = NULL;
	cmd->argv = new_argv;
	return (0);
}

static int	add_redirection(t_command *cmd, t_node_type type, char *filename)
{
	t_redirect	*redir;
	t_redirect	*tmp;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (-1);

	redir->type = type;
	redir->filename = ft_strdup(filename);
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
	return (0);
}

t_treenode *parse_simple_command(t_token *tokens[], int start, int end)
{
	t_treenode	*node;
	t_command	*cmd;
	int			i;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	i = start;
	while (i <= end)
	{
		if (tokens[i]->type == TOKEN_WORD)
		{
			if (add_argument(cmd, tokens[i]->content) != 0)
				return (NULL); // free needed
		}
		else if (is_redirection(tokens[i]->type))
		{
			if (i + 1 > end || tokens[i + 1]->type != TOKEN_WORD)
				return (NULL);
			if (add_redirection(cmd, tokens[i]->type, tokens[i + 1]->content) != 0)
				return (NULL);
			i++;
		}
		else
			return (NULL);
		i++;
	}
	//TODO: Error no command
	if (!cmd->argv || !cmd->argv[0])
	{
		//free_command(cmd);
		return (NULL);
	}

	node = new_node(TOKEN_COMMAND);
	if (!node)
		return (NULL);
	node->cmd = cmd;
	return (node);
}

t_treenode	*build_tree(t_token *tokens[], int start, int end)
{
	t_treenode	*node;
	t_treenode	*left;
	t_treenode	*right;
	int			i;

	if (start > end)
		return (NULL);

	i = start;
	while (i <= end)
	{
		if (tokens[i]->type == TOKEN_AND || tokens[i]->type == TOKEN_OR)
		{
			node = new_node(tokens[i]->type);
			if (!node)
				return (NULL);
			left = build_tree(tokens, start, i - 1);
			right = build_tree(tokens, i + 1, end);
			if (!left || !right)
				return (NULL);
			node->left = left;
			node->right = right;
			return (node);
		}
		i++;
	}
	i = start;
	while (i <= end)
	{
		if (tokens[i]->type == TOKEN_PIPE)
		{
			node = new_node(TOKEN_PIPE);
			if (!node)
				return (NULL);
			left = build_tree(tokens, start, i - 1);
			right = build_tree(tokens, i + 1, end);
			if (!left || !right)
				return (NULL);
			node->left = left;
			node->right = right;
			return (node);
		}
		i++;
	}
	return (parse_simple_command(tokens, start, end));
}

