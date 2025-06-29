#include "../include/minishell.h"

static t_node_type	get_token_type(const char *str)
{
    if (strcmp(str, "|") == 0)
        return TOKEN_PIPE;
    if (strcmp(str, "<") == 0)
        return TOKEN_REDIRECT_IN;
    if (strcmp(str, ">") == 0)
        return TOKEN_REDIRECT_OUT;
    return TOKEN_WORD;
}

static t_treenode *new_node(t_node_type type, char *content) {
	t_treenode *node = malloc(sizeof(t_treenode));
	node->type = type;
	node->content = ft_strdup(content);
	node->left = NULL;
	node->right = NULL;
	return node;
}

static int get_priority(t_node_type type) {
	if (type == TOKEN_PIPE)
		return 1;
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT)
		return 2;
	return 3;
}

static t_treenode *build_tree(t_treenode **tokens, int start, int end) {
	if (start > end)
		return NULL;
	if (start == end)
		return tokens[start];

	int op = -1;
	int min_prio = 1000;

	for (int i = end; i >= start; i--) {
		int prio = get_priority(tokens[i]->type);
		if (prio <= min_prio) {
			min_prio = prio;
			op = i;
		}
	}

	t_treenode *root = tokens[op];
	if (root->type == TOKEN_REDIRECT_IN || root->type == TOKEN_REDIRECT_OUT) {
		if (op + 1 > end)
			return root;

		root->left = tokens[op + 1];

		if (op + 2 <= end) {
			root->right = build_tree(tokens, op + 2, end);
		} else {
			root->right = build_tree(tokens, start, op - 1);
		}
		return root;
	}

	root->left = build_tree(tokens, start, op - 1);
	root->right = build_tree(tokens, op + 1, end);
	return root;
}

static int group_tokens(t_treenode **tokens, int count) {
	t_treenode *grouped[256];
	int i = 0, j = 0;

	while (i < count) {
		if (i > 0 && (tokens[i - 1]->type == TOKEN_REDIRECT_IN || tokens[i - 1]->type == TOKEN_REDIRECT_OUT)) {
			grouped[j++] = tokens[i++];
			continue;
		}

		if (tokens[i]->type == TOKEN_WORD) {
			char buffer[1024] = {0};
			while (i < count && tokens[i]->type == TOKEN_WORD) {
				if (i > 0 && (tokens[i - 1]->type == TOKEN_REDIRECT_IN || tokens[i - 1]->type == TOKEN_REDIRECT_OUT))
					break;
				ft_strcat(buffer, tokens[i]->content);
				if (i + 1 < count && tokens[i + 1]->type == TOKEN_WORD)
					ft_strcat(buffer, " ");
				i++;
			}
			grouped[j++] = new_node(TOKEN_COMMAND, buffer);
		} else {
			grouped[j++] = tokens[i++];
		}
	}

	for (i = 0; i < j; i++)
		tokens[i] = grouped[i];
	return j;
}

void create_tree(char *line, t_treenode **tree) {
	t_treenode *tokens[256];
	int count = 0;
	int i = 0;

	while (line[i]) {
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (!line[i])
			break;
		if (line[i] == '|' || line[i] == '<' || line[i] == '>') {
			char sym[2] = {line[i], '\0'};
			tokens[count++] = new_node(get_token_type(sym), sym);
			i++;
			continue;
		}
		int start = i;
		while (line[i] && line[i] != ' ' && line[i] != '\t' &&
			   line[i] != '|' && line[i] != '<' && line[i] != '>')
			i++;

		int len = i - start;
		if (len > 0) {
			char word[256];
			strncpy(word, &line[start], len);
			word[len] = '\0';
			tokens[count++] = new_node(get_token_type(word), word);
		}
	}
	count = group_tokens(tokens, count);
	*tree = build_tree(tokens, 0, count - 1);
}

void p_treenode(t_treenode *node) {
    if (!node)
        return;

    printf("Node Type: %d, Content: %s\n", node->type, node->content);
    p_treenode(node->left);
    p_treenode(node->right);
}

void print_tree(t_treenode *node, int depth, char side) {
	if (!node)
		return;
	print_tree(node->right, depth + 1, 'R');
	for (int i = 0; i < depth; i++)
		printf("           ");
	printf("%c─[%d] %s\n", side, node->type, node->content);
	print_tree(node->left, depth + 1, 'L');
}

int parse_line(char *line)
{
    t_treenode *tree;
    tree = NULL;
    if (line == NULL || line[0] == '\0')
        return (0);

    create_tree(line, &tree);

    print_tree(tree, 0, 's'); // For Debug, this prints visually the tree structure
    //p_treenode(tree); //For Debug,  Print the tree structure as text
    return (1);
}
//< infile ls -l | wc -l | grep al > outfile
