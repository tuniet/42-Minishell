/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:18:47 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:18:48 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_tokens(t_token **tokens, int count);

static t_node_type	get_token_type(const char *str)
{
	if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_APPEND);
	if (ft_strcmp(str, "&&") == 0)
		return (TOKEN_AND);
	if (ft_strcmp(str, "||") == 0)
		return (TOKEN_OR);
	if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (ft_strcmp(str, "&") == 0)
		return (TOKEN_AMPERSANT);
	return (TOKEN_WORD);
}

t_token	*new_token(t_node_type type, char *content)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->type = type;
	node->content = ft_strdup(content);
	if (!(node->content))
	{
		free(node);
		return (NULL);
	}
	return (node);
}

char	*get_token(char *c, t_token **token)
{
	char	*end;
	char	*cpToken;

	while (*c == ' ' || *c == '\t')
		c++;
	if (*c == '\0')
		return (NULL);
	end = get_token_end(c);
	cpToken = (char *)malloc(end - c + 1);
	if (!cpToken)
		return (NULL);
	ft_strlcpy(cpToken, c, (end - c) + 1);
	*token = new_token(get_token_type(cpToken), cpToken);
	if (!(*token))
	{
		free(cpToken);
		return (NULL);
	}
	free(cpToken);
	return (end);
}

int	tokenize(char *line, t_token **tokens)
{
	int			iCount;
	t_token		*tToken;
	char		*pcAux;

	iCount = 0;
	pcAux = line;
	while (*pcAux && iCount < MAX_TOKENS)
	{
		tToken = NULL;
		pcAux = get_token(pcAux, &tToken);
		if (!pcAux)
			return (-1);
		tokens[iCount++] = tToken;
		while (*pcAux == ' ' || *pcAux == '\t')
			pcAux++;
	}
	tokens[MAX_TOKENS] = NULL;
	return (iCount);
}

//TODO : TEST IT
// < infile ls -l | wc -l | grep al > outfile
// > out cat < Makefile

