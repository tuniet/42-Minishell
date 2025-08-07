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

// FUNCTIONS DEFS
typedef int (*is_delimiter_t)(int c);

// STRUCTS
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
	TOKEN_WORD = 0,
	TOKEN_PIPE = 1,
	TOKEN_REDIRECT_IN = 2,
	TOKEN_REDIRECT_OUT = 3,
	TOKEN_APPEND = 4,
	TOKEN_HEREDOC = 5,
	//TOKEN_SEMICOLON,
	TOKEN_AND = 6,
	TOKEN_OR = 7,
	TOKEN_VARIABLE = 8,//TODO : implement at executing time
	TOKEN_ENV_VAR = 9,//TODO : implement at executing time
	TOKEN_COMMAND = 10,//TODO : implement at executing time
	TOKEN_AMPERSANT = 11//TODO : implement at executing time
} t_node_type;

// TREE STRUCTS
typedef struct s_redirect {
	t_node_type			type;
	char				*filename;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command {
	char		**argv;
	t_redirect	*redirects;
}	t_command;

typedef struct s_treenode {
	t_node_type			type;
	struct s_treenode	*left;
	struct s_treenode	*right;
	t_command			*cmd;
}	t_treenode;

// TOKENS STRUCT
typedef struct s_token {
	t_node_type	type;
	char		*content;
}	t_token;

// Prompt functions
int set_prompt(t_data* data);
int get_prompt(char **p, t_data *data);
int init_data(t_data *data, char *envp[]);
char		*mini_getenv(char *var, char *envp[]);

// tokenizer functions
int			tokenize(char *line, t_token **tokens);
void		free_tokens(t_token *tokens);
int 		parse_line(char *line, t_data *data);

// parsing functions
t_treenode	*build_tree(t_token *tokens[], int start, int end);
void		print_tree(t_treenode *node, int level);
// parsing utils:
t_command	*init_command(void);
void		print_tree(t_treenode *node, int level);
int			is_redirection(t_node_type type);
t_treenode	*new_node(t_node_type type);

// Aux functions
int			is_metachar(int c);
char		*get_token_end(char *line);

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
int		execute_tree(t_treenode *node, char **envp);
// Execute Utils
char	*find_executable(char *command, char **envp);

#endif
