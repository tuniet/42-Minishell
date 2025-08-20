/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:22:51 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 01:54:14 by antoniof         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../include/minishell.h"

static void sigint_handler_heredoc(int sig)
{
    (void)sig;
    write(1, "\n", 1);            // print newline
    signal(SIGINT, SIG_DFL);      // reset handler to default
    kill(getpid(), SIGINT);       // send SIGINT to myself
}

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
	name = strndup(s + start, *i - start);
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
			part = strndup(tok + start, i - start);
		}
		if (!part)
			return (free(res), NULL);
		res = strjoin_free(res, part);
	}
	return (res);
}

static int heredoc_child(char *delimiter, t_data *data, int pipefd[2])
{
    //char *line;

    signal(SIGINT, sigint_handler_heredoc); // simple setup

    close(pipefd[0]);

while (1)
{
    char *raw_line = readline("> ");
    if (!raw_line) // Ctrl+D
    {
        close(pipefd[1]);
        exit(0);
    }

    if (strcmp(raw_line, delimiter) == 0)
    {
        free(raw_line);
        close(pipefd[1]);
        exit(0);
    }

    char *line = expand_line(raw_line, data->envp, data->i_exit);
    free(raw_line); // free immediately after expanding
    if (!line) // allocation failed
    {
        close(pipefd[1]);
        exit(1);
    }

    write(pipefd[1], line, strlen(line));
    write(pipefd[1], "\n", 1);
    free(line);
}
    close(pipefd[1]);
    return (0); // normal end of heredoc
}

int heredoc(char *delimiter, t_data *data)
{
    int     pipefd[2];
    pid_t   pid;
    int     status;

    if (pipe(pipefd) < 0)
        return (-1);

    pid = fork();
    if (pid == -1)
        return (-1);

    if (pid == 0)
        heredoc_child(delimiter, data, pipefd);

    // parent ignores SIGINT during heredoc
    signal(SIGINT, SIG_IGN);

    close(pipefd[1]);
    waitpid(pid, &status, 0);

    // restore normal signals
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
