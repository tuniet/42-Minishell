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
	char	*cp_token;

	while (*c == ' ' || *c == '\t')
		c++;
	if (*c == '\0')
		return (NULL);
	end = get_token_end(c);
	cp_token = (char *)malloc(end - c + 1);
	if (!cp_token)
		return (NULL);
	ft_strlcpy(cp_token, c, (end - c) + 1);
	*token = new_token(get_token_type(cp_token), cp_token);
	if (!(*token))
	{
		free(cp_token);
		return (NULL);
	}
	free(cp_token);
	return (end);
}

int	tokenize(char *line, t_token **tokens)
{
	int			i_count;
	t_token		*token;
	char		*pc_aux;

	i_count = 0;
	pc_aux = line;
	while (*pc_aux && i_count < MAX_TOKENS)
	{
		token = NULL;
		pc_aux = get_token(pc_aux, &token);
		if (!pc_aux)
			return (-1);
		tokens[i_count++] = token;
		while (*pc_aux == ' ' || *pc_aux == '\t')
			pc_aux++;
	}
	tokens[MAX_TOKENS] = NULL;
	return (i_count);
}
