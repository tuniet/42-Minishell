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

char	*expand_single_quote(const char *s, int *i)
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

char	*expand_double_quote(const char *s, int *i, char **envp, int st)
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

char	*expand_other(const char *s, int *i, char **envp, int st)
{
	char	tmp[2];

	if (s[*i] == '$')
		return (expand_variable(s, i, envp, st));
	tmp[0] = s[(*i)++];
	tmp[1] = '\0';
	return (strdup(tmp));
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
		if (!argv)
			free_argv(exp);
		i++;
	}
	return (argv);
}
