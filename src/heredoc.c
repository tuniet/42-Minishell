/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:22:51 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/19 21:59:17 by antoniof         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

static char	*expand_line(char *tok, char **envp, int i_exit)
{
	char	*res;
	char	*part;
	int		i;
	int		start;

	res = strdup("");
	if (!res)
		return (free(tok), NULL);
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
			part = strndup(tok + start, i - start);
		}
		if (!part)
			return (free(res), free(tok), NULL);
		res = strjoin_free(res, part);
	}
	return (free(tok), res);
}

int	heredoc(char *delimiter, t_data *data)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) < 0)
		return (free(delimiter), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		line = expand_line(line, data->envp, data->i_exit);
		if (line)
		{
			write(pipefd[1], line, strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
	}
	free(delimiter);
	close(pipefd[1]);
	return (pipefd[0]);
}
