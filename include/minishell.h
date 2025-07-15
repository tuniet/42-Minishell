#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include "colors.h"
# include <fcntl.h>
# include <stdio.h>
#include <readline/history.h>
#include <readline/readline.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_data{
	char* prompt;
	char* pwd;
	char* home;
	char* user;
	char* host;
	char** envp;
} t_data;

typedef enum e_node_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	//TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SEMICOLON,
	//TOKEN_AND,
	//TOKEN_OR,
	TOKEN_VARIABLE,
	TOKEN_ENV_VAR,
	TOKEN_COMMAND
} t_node_type;

typedef struct s_treenode
{
	t_node_type			type;
	char 				*content;
	struct s_treenode 	*left;
	struct s_treenode 	*right;

} t_treenode;

// Prompt functions
int set_prompt(t_data* data);
int get_prompt(char **p, t_data *data);
int init_data(t_data *data, char *envp[]);
char		*mini_getenv(char *var, char *envp[]);

// tokenizer functions
t_treenode		*tokenize(char *line);
void		free_tokens(t_treenode *tokens);
int 		parse_line(char *line, t_data *data);

// String functions
char	*ft_strcpy(char *dest, char *src);
char	*ft_strcat(char *dest, char *src);
char	*ft_strdup(const char *src);
size_t	ft_strlen(const char *s);
int ft_strncmp(const char *s1, const char *s2, size_t n);
void *ft_memcpy(void *dest, const void *src, size_t n);

// Signal functions
void setup_signals(void);

// Execute tree
void		execute_tree(t_treenode *node, char **envp);

#endif
