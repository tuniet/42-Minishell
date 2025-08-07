#include "../include/minishell.h"

void	print_tokens(t_token **tokens, int count);

static t_node_type	get_token_type(const char *str)
{
	if (strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (strcmp(str, ">>") == 0)
		return (TOKEN_APPEND);
	if (strcmp(str, "&&") == 0)
		return (TOKEN_AND);
	if (strcmp(str, "||") == 0)
		return (TOKEN_OR);
	if (strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	if (strcmp(str, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (strcmp(str, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (strcmp(str, "&") == 0)
		return (TOKEN_AMPERSANT);
	return (TOKEN_WORD);
}

t_token	*new_token(t_node_type type, char *content)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);//TODO: Exit program. Malloc error
	node->type = type;
	node->content = ft_strdup(content);
	if (!(node->content))
	{
		free(node);
		return (NULL);//TODO: Exit program. Malloc error
	}
	return (node);
}

char	*get_token(char *c, t_token **token)
{
	char	*end;
	char	*cpToken;

	while (*c == ' ' || *c == '\t')
		c++;
	if (*c == '\0')
		return (c);
	//printf("start : %c\n", *c);
	end = get_token_end(c);
	//printf("end : %c\n", *(end));
	cpToken = (char *)malloc(end - c + 1);
	if (!cpToken)
		return (NULL); //Call exit program (malloc error : -2)
	strlcpy(cpToken, c, (end - c) + 1);
	//printf("Token: %s\n", cpToken);
	*token = new_token(get_token_type(cpToken), cpToken);
	free(cpToken);
	return (end);
}

int	tokenize(char *line, t_token **tokens)
{
	int			iCount;
	t_token		*tToken;
	char		*pcAux;

	iCount = 0;
	pcAux = line;
	while (*pcAux)
	{
		tToken = NULL;
		pcAux = get_token(pcAux, &tToken);
		if (tToken != NULL)
			tokens[iCount++] = tToken;
		while (*pcAux == ' ' || *pcAux == '\t')
			pcAux++;
	}
	return (iCount);
}

int	create_tree(char *line, t_treenode **tree)
{
	//t_treenode	**tokens;
	//TODO: do it with realloc instead!
	t_token		*tokens[100];
	int			count;
	int			i;

	i = 0;
	count = tokenize(line, tokens);
	//NOTE: For debugging tokenizer
	print_tokens(tokens, count);
	//
	*tree = build_tree(tokens, 0, count -1);
	if (!(*tree))
		return (1);
	return (0);
}

void	print_tokens(t_token **tokens, int count)
{
	for (int i = 0; i < count; i++)
		printf("[%d] Type: %d | Value: %s\n", i, tokens[i]->type, tokens[i]->content);
}

int	parse_line(char *line, t_data *data)
{
	t_treenode	*tree;
	int			iRet;

	iRet = 0;
	tree = NULL;
	if (line == NULL || line[0] == '\0')
		return (0);
	iRet = create_tree(line, &tree);
	if (iRet == 0)
        print_tree(tree, 0);
	else
	{
		printf("Error ast creation!\n");
		return (0);
	}

	//print_tree(tree, 0, 's'); // For Debug, this prints visually the tree structure
	//p_treenode(tree); //For Debug,  Print the tree structure as text

	//TODO: to implement and test
	execute_tree(tree, data->envp);
	return (1);
}

//TODO : TEST IT
// < infile ls -l | wc -l | grep al > outfile
// > out cat < Makefile

