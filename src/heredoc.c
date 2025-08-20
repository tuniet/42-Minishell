/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:28:14 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 14:28:17 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_variable_(const char *s, int *i, char **envp, int i_exit)
{
	int		start;
	char	*name;
	char	*value;
	char	buf[12];

	(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		snprintf(buf, sizeof(buf), "%d", i_exit);
		return (ft_strdup(buf));
	}
	start = *i;
	while (s[*i] && (isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	name = ft_strndup(s + start, *i - start);
	if (!name)
		return (NULL);
	value = mini_getenv(name, envp);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*expand_line(char *tok, char **envp, int i_exit)
{
	char	*res;
	char	*part;
	int		i;
	int		start;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (tok[i])
	{
		if (tok[i] == '$')
			part = expand_variable_(tok, &i, envp, i_exit);
		else
		{
			start = i;
			while (tok[i] && tok[i] != '$')
				i++;
			part = ft_strndup(tok + start, i - start);
		}
		if (!part)
			return (free(res), NULL);
		res = strjoin_free(res, part);
	}
	return (res);
}

static int	process_heredoc_line(char *delimiter, t_data *data, int pipefd)
{
	char	*raw_line;
	char	*line;

	raw_line = readline("> ");
	if (!raw_line)
	{
		close(pipefd);
		exit(0);
	}
	if (ft_strcmp(raw_line, delimiter) == 0)
	{
		free(raw_line);
		close(pipefd);
		exit(0);
	}
	line = expand_line(raw_line, data->envp, data->i_exit);
	free(raw_line);
	if (!line)
	{
		close(pipefd);
		exit(1);
	}
	write(pipefd, line, ft_strlen(line));
	write(pipefd, "\n", 1);
	return (free(line), 0);
}

static int	heredoc_child(char *delimiter, t_data *data, int pipefd[2])
{
	signal(SIGINT, sigint_handler_heredoc);
	close(pipefd[0]);
	while (1)
	{
		process_heredoc_line(delimiter, data, pipefd[1]);
	}
	close(pipefd[1]);
	return (0);
}

int	heredoc(char *delimiter, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) < 0)
		return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		heredoc_child(delimiter, data, pipefd);
	signal(SIGINT, SIG_IGN);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		data->i_exit = 130;
		close(pipefd[0]);
		return (-1);
	}
	data->i_exit = WEXITSTATUS(status);
	return (pipefd[0]);
}
