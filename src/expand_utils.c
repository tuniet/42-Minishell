/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:31:08 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 14:31:09 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*strjoin_free(char *s1, char *s2)
{
	char	*res;
	size_t	len1;
	size_t	len2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc(len1 + len2 + 1);
	if (!res)
		return (free(s1), free(s2), NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	free(s1);
	free(s2);
	return (res);
}

char	*expand_token_(char *tok, char **envp, int i_exit)
{
	char	*res;
	char	*part;
	int		i;

	i = 0;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (tok[i])
	{
		if (tok[i] == '\'')
			part = expand_single_quote(tok, &i);
		else if (tok[i] == '"')
			part = expand_double_quote(tok, &i, envp, i_exit);
		else
			part = expand_other(tok, &i, envp, i_exit);
		if (!part)
			return (free(res), NULL);
		res = strjoin_free(res, part);
	}
	return (res);
}

char	*expand_variable(const char *s, int *i, char **envp, int st)
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
		return (ft_strdup(buf));
	}
	if (!(ft_isalnum(s[*i]) || s[*i] == '_'))
		return (ft_strdup("$"));
	start = *i;
	while (s[*i] && (isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	name = ft_strndup(s + start, *i - start);
	value = mini_getenv(name, envp);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*expand_dispatch(char *tok, int *i, t_expand_ctx *ctx)
{
	if (tok[*i] == '\'')
	{
		ctx->had_q = 1;
		return (expand_single_quote(tok, i));
	}
	if (tok[*i] == '"')
	{
		ctx->had_q = 1;
		return (expand_double_quote(tok, i, ctx->envp, ctx->status));
	}
	return (expand_other(tok, i, ctx->envp, ctx->status));
}

static char	*expand_token_build(char *tok, t_expand_ctx *ctx)
{
	char	*res;
	int		i;
	char	*part;

	i = 0;
	res = ft_strdup("");
	while (tok[i])
	{
		part = expand_dispatch(tok, &i, ctx);
		res = strjoin_free(res, part);
	}
	return (res);
}
