/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:16:30 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:16:32 by antoniof         ###   ########.fr       */
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

char	*expand_token_(char *tok, char **envp, int i_exit)
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
