/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:14:21 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:14:23 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "colors.h"
# include <ctype.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <term.h>
# include <unistd.h>
# include "../libft/libft.h"
// DEFINES
# define MAX_TOKENS 100

// ENUM
typedef enum e_node_type
{
	TOKEN_WORD = 0,
	TOKEN_PIPE = 1,
	TOKEN_REDIRECT_IN = 2,
	TOKEN_REDIRECT_OUT = 3,
	TOKEN_APPEND = 4,
	TOKEN_HEREDOC = 5,
	TOKEN_AND = 6,
	TOKEN_OR = 7,
	TOKEN_VARIABLE = 8,
	TOKEN_ENV_VAR = 9,
	TOKEN_COMMAND = 10,
	TOKEN_AMPERSANT = 11,
}						t_node_type;

typedef struct s_token
{
	t_node_type			type;
	char				*content;
}						t_token;

typedef struct s_redirect
{
	t_node_type			type;
	char				*filename;
	struct s_redirect	*next;
	int					hered_fd;
}						t_redirect;

typedef struct s_command
{
	t_token				**argv;
	int					argc;
	int					capacity;
	t_redirect			*redirects;
}						t_command;

typedef struct s_treenode
{
	t_node_type			type;
	struct s_treenode	*left;
	struct s_treenode	*right;
	t_command			*cmd;
}						t_treenode;

typedef struct s_data
{
	char				*prompt;
	char				*pwd;
	char				*home;
	char				*user;
	char				*host;
	char				**envp;
	t_token				*tokens[MAX_TOKENS];
	int					tokens_size;
	t_treenode			*ast_root;
	int					i_exit;
}						t_data;

// CTX STRUCTS
typedef struct s_parse_ctx
{
	t_token		**tokens;
	int			start;
	int			end;
	t_data		*data;
}	t_parse_ctx;

typedef struct s_expand_ctx
{
	char	**envp;
	int		status;
	int		had_q;
}	t_expand_ctx;

// Prompt functions
int						set_prompt(t_data *data);
int						get_prompt(char **p, t_data *data);
int						init_data(t_data *data, char *envp[]);
char					*mini_getenv(char *var, char *envp[]);
int						print_prompt(char *prompt, char *user, char *host,
							t_data *data);

// tokenizer.c
int						tokenize(char *line, t_token **tokens);
int						parse_line(char *line, t_data *data);

// tree.c
t_treenode				*build_tree(t_token *tokens[], int start,
							int end, t_data *data);
void					print_tree(t_treenode *node, int level);
// tree_utils.c:
t_command				*init_command(void);
int						is_redirection(t_node_type type);
t_treenode				*new_node(t_node_type type);
t_treenode				*build_binary_node(t_token *tokens[], int start,
							int end, int op_index, t_data *data);

// tree_utils2.c:
int						add_redirection(t_command *cmd, t_node_type type,
							char *filename, t_data *data);
int						add_argument(t_command *cmd, t_token *arg);

// String functions
char					*ft_strcpy(char *dest, char *src);
char					*ft_strcat(char *dest, char *src);
char					*ft_strdup(const char *src);
size_t					ft_strlen(const char *s);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
void					*ft_memcpy(void *dest, const void *src, size_t n);
int						ft_strcmp(const char *s1, const char *s2);
size_t					ft_strlcpy(char *dst, const char *src, size_t size);

// execute.c
int						execute_tree(t_treenode *node, char **envp,
							t_data *data);
int						execute_command_node(t_treenode *node, char **envp,
							t_data *data);
int						execute_logical_node(t_treenode *node, char **envp,
							t_data *data);
// execute_utils.c
char					*find_executable(char *command, char **envp);

// mem.c
void					mini_free(void **ptr);

// free_tree.c
void					free_redirects(t_redirect *redir);
void					free_command(t_command *cmd);

// free.c
void					free_tokens(t_token **tokens);
void					free_tree(t_treenode *node);
void					free_all(t_data *data, int flag);

// expand.c
char					**expand(t_token **tokens, char **envp, int i_exit);
char					*expand_other(const char *s, int *i, char **envp,
							int st);
char					*expand_double_quote(const char *s, int *i, char **envp,
							int st);
char					*expand_single_quote(const char *s, int *i);

// expand_wildcard.c
char					**expand_wildcards(const char *pattern);

// expand_utils.c
char					*strjoin_free(char *s1, char *s2);
char					*expand_token_(char *tok, char **envp, int i_exit);
char					*expand_variable(const char *s, int *i, char **envp,
							int st);
char					*expand_dispatch(char *tok, int *i, t_expand_ctx *ctx);

// argv_funcs.c
void					free_argv(char **argv);
int						argv_len(char **argv);
char					**copy_argv(char **src, int *index, char **res);
char					**argv_join(char **argv, char **exp);

// Aux functions
int						is_metachar(int c);
char					*get_token_end(char *line);

// ft_split.c
char					**ft_split(const char *s, char c);

// builtins.c
int						is_builtin(const char *cmd);
int						execute_builtin(char **argv, t_data *data);
int						mini_echo(char **argv);
int						mini_pwd(void);
int						mini_env(t_data *data, char **argv);
int						mini_cd(char **argv, t_data *data);
int						mini_exit(char **argv);
int						mini_export(char **argv, t_data *data);
int						mini_unset(char **argv, t_data *data);
int						find_env_index(char **envp, const char *name);

// envp.c
int						update_envp(char **envp, char *var, char *new_value);

// signals.c
void					handle_sigint(int sig);
void					handle_sigquit(int sig);
void					setup_signals(void);
// heredoc.c
int						heredoc(char *delimiter, t_data *data);

#endif
