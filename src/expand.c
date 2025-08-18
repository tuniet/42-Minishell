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
	char	*ret;
	char	**aux;
	int		j;
	if (s[*i] == '$')
	{
		ret = expand_variable(s, i, envp, st);
		aux = ft_split(ret, ' ');
		free(ret);
		ret = ft_strdup("");
		j = 0;
		while(aux[j])
		{
			ret = strjoin_free(ret, aux[j]);
			if(aux[j + 1])
				ret = strjoin_free(ret, ft_strdup(" "));
			j++;
		}
		free(aux);
		return (ret);
	}
	tmp[0] = s[(*i)++];
	tmp[1] = '\0';
	return (ft_strdup(tmp));
}

static char	**expand_token(char *tok, char **envp, int st)
{
	char			*res;
	int				i;
	char			*part;
	char			**ret;
	t_expand_ctx	ctx;

	i = 0;
	ctx.envp = envp;
	ctx.status = st;
	ctx.had_q = 0;
	res = strdup("");
	while (tok[i])
	{
		part = expand_dispatch(tok, &i, &ctx);
		res = strjoin_free(res, part);
	}
	if (!ctx.had_q && strchr(res, '*'))
	{
		ret = expand_wildcards(res);
		free(res);
		return (ret);
	}
	printf("res : %s\n", res);
	ret = malloc(sizeof(char *) * 2);
	ret[0] = ft_strdup(res);
	ret[1] = NULL;
	//ret = ft_split(res, ' ');
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
