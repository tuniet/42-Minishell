/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:22:51 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 20:22:52 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*expand_variable_(const char *s, int *i, char **envp, int iExit)
{
	int		start;
	char	*name;
	char	*value;
	char	buf[12];

	(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		snprintf(buf, sizeof(buf), "%d", iExit);
		return (strdup(buf));
	}
	start = *i;
	while (s[*i] && (isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	name = strndup(s + start, *i - start);
	if (!name)
		return (NULL);
	value = mini_getenv(name, envp);
	free(name);
	if (!value)
		return (strdup(""));
	return (strdup(value));
}

static char	*expand_line(char *tok, char **envp, int iExit)
{
	char	*res;
	char	*part;
	int		i;
	int		start;

	res = strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (tok[i])
	{
		if (tok[i] == '$')
			part = expand_variable_(tok, &i, envp, iExit);
		else
		{
			start = i;
			while (tok[i] && tok[i] != '$')
				i++;
			part = strndup(tok + start, i - start);
		}
		if (!part)
			return (free(res), NULL);
		res = strjoin_free(res, part);
	}
	return (res);
}

int	heredoc(char *delimiter, t_data *data)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		line = expand_line(line, data->envp, data->iExit);
		if (line)
		{
			write(pipefd[1], line, strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
