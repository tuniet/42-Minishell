/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:16:24 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:16:25 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <dirent.h>
#include <fnmatch.h>

char	*strjoin_free(char *s1, char *s2)
{
	char	*res;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2 ? strdup(s2) : NULL);
	if (!s2)
		return (s1);
	len1 = strlen(s1);
	len2 = strlen(s2);
	res = malloc(len1 + len2 + 1);
	if (!res)
		return (free(s1), free(s2), NULL);
	memcpy(res, s1, len1);
	memcpy(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	free(s1);
	free(s2);
	return (res);
}

static char	*expand_variable(const char *s, int *i, char **envp, int st)
{
	int		start;
	char	*name;
	char	*value;
	char	buf[12];

	(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		snprintf(buf, sizeof(buf), "%d", st);
		return (strdup(buf));
	}
	start = *i;
	while (s[*i] && (isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	name = strndup(s + start, *i - start);
	value = mini_getenv(name, envp);
	free(name);
	if (!value)
		return (strdup(""));
	return (strdup(value));
}

static char	*expand_single_quote(const char *s, int *i)
{
	int		start;
	char	*part;

	(*i)++;
	start = *i;
	while (s[*i] && s[*i] != '\'')
		(*i)++;
	part = strndup(s + start, *i - start);
	if (s[*i] == '\'')
		(*i)++;
	return (part);
}

static char	*expand_double_quote(const char *s, int *i, char **envp, int st)
{
	char	*res;
	char	tmp[2];

	(*i)++;
	res = strdup("");
	while (s[*i] && s[*i] != '"')
	{
		if (s[*i] == '$')
			res = strjoin_free(res, expand_variable(s, i, envp, st));
		else
		{
			tmp[0] = s[(*i)++];
			tmp[1] = '\0';
			res = strjoin_free(res, strdup(tmp));
		}
	}
	if (s[*i] == '"')
		(*i)++;
	return (res);
}

static char	*expand_other(const char *s, int *i, char **envp, int st)
{
	char	tmp[2];

	if (s[*i] == '$')
		return (expand_variable(s, i, envp, st));
	tmp[0] = s[(*i)++];
	tmp[1] = '\0';
	return (strdup(tmp));
}

static char	**expand_wildcards(const char *pattern)
{
	DIR				*dir;
	struct dirent	*ent;
	char			**matches;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (ft_split(pattern, ' '));
	matches = NULL;
	count = 0;
	while ((ent = readdir(dir)))
	{
		if (fnmatch(pattern, ent->d_name, 0) == 0)
		{
			matches = realloc(matches, sizeof(char *) * (count + 2));
			matches[count++] = strdup(ent->d_name);
			matches[count] = NULL;
		}
	}
	closedir(dir);
	if (count == 0)
		return (ft_split(pattern, ' '));
	return (matches);
}

static char	**expand_token(char *tok, char **envp, int st)
{
	char	*res;
	int		i;
	int		had_q;
	char	*part;
	char	**ret;

	i = 0;
	had_q = 0;
	res = strdup("");
	while (tok[i])
	{
		if (tok[i] == '\'')
			part = expand_single_quote(tok, &i), had_q = 1;
		else if (tok[i] == '"')
			part = expand_double_quote(tok, &i, envp, st), had_q = 1;
		else
			part = expand_other(tok, &i, envp, st);
		res = strjoin_free(res, part);
	}
	if (!had_q && strchr(res, '*'))
		return (expand_wildcards(res));
	ret = ft_split(res, ' ');
	free(res);
	return (ret);
}

static char	**copy_argv(char **src, int *index, char **res)
{
	int	i;

	i = 0;
	while (src && src[i])
	{
		res[*index] = strdup(src[i]);
		if (!res[*index])
		{
			free_argv(res);
			return (NULL);
		}
		(*index)++;
		i++;
	}
	return (res);
}

static char	**argv_join(char **argv, char **exp)
{
	int		size1;
	int		size2;
	int		i;
	int		j;
	char	**res;

	size1 = argv_len(argv);
	size2 = argv_len(exp);
	res = malloc(sizeof(char *) * (size1 + size2 + 1));
	if (!res)
		return (NULL);

	i = 0;
	if (!copy_argv(argv, &i, res))
		return (NULL);
	j = 0;
	if (!copy_argv(exp, &i, res))
		return (NULL);

	res[i] = NULL;
	free_argv(argv);
	free_argv(exp);
	return (res);
}

char	*expand_token_(char *tok, char **envp, int iExit)
{
    char	*res;
    char	*part;
    int		i;

    i = 0;
    res = strdup("");
    if (!res)
        return (NULL);
    while (tok[i])
    {
        if (tok[i] == '\'')
            part = expand_single_quote(tok, &i);
        else if (tok[i] == '"')
            part = expand_double_quote(tok, &i, envp, iExit);
        else
            part = expand_other(tok, &i, envp, iExit);
        if (!part)
            return (free(res), NULL);
        res = strjoin_free(res, part);
    }
    return (res);
}

char	**expand(t_token **tokens, char **envp, int iExit)
{
	char	**argv;
	char	**exp;
	int		i;

	argv = NULL;
	i = 0;
	while (tokens[i])
	{
		exp = expand_token(tokens[i]->content, envp, iExit);
		argv = argv_join(argv, exp);
		if(!argv)
		{
			free_argv(exp);
		}
		i++;
	}
	return (argv);
}
