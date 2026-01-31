/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:15:20 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/22 16:37:19 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_arg(char **argv, int *i)
{
	size_t	len;

	while (argv[*i])
	{
		len = ft_strlen(argv[*i]);
		if ((argv[*i][0] == '\'' && argv[*i][len - 1] == '\'')
			|| (argv[*i][0] == '"' && argv[*i][len - 1] == '"'))
		{
			argv[*i][len - 1] = '\0';
			argv[*i]++;
		}
		printf("%s", argv[*i]);
		if (argv[*i + 1])
			printf(" ");
		(*i)++;
	}
}

static int	apply_echo_redirections(t_redirect *redir_list, t_data *data)
{
	t_redirect	*redir;
	int			fd;

	redir = redir_list;
	while (redir)
	{
		fd = open_redir(redir, data);
		if (fd < 0)
			return (print_echo_error(redir->filename, strerror(errno)), -1);
		if (redir->type == TOKEN_REDIRECT_OUT || redir->type == TOKEN_APPEND)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

int	mini_echo(t_treenode *node, t_data *data, char **argv)
{
	int		i;
	int		newline_;
	pid_t	e;
	int		status;

	i = 1;
	newline_ = 1;
	e = fork();
	if (e == 0)
	{
		if (apply_echo_redirections(node->cmd->redirects, data) != 0)
			exit(1);
		if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
		{
			newline_ = 0;
			i = 2;
		}
		print_arg(argv, &i);
		if (newline_)
			printf("\n");
		exit(0);
	}
	waitpid(e, &status, 0);
	return (WEXITSTATUS(status));
}

int	mini_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}
