/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:15:07 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:15:09 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_metachar(int c)
{
	if (c == '\0' || c == '\t' || c == ' '
		|| c == '&' || c == '|'
		|| c == '<' || c == '>')
		return (1);
	return (0);
}

char	*get_token_end(char *line)
{
	char	quote;

	if (*line == '\0')
		return (line);
	if (*line == '|' || *line == '&' || *line == '<' || *line == '>')
	{
		if (*(line + 1) == *line)
			return (line + 2);
		return (line + 1);
	}
	while (*line && !is_metachar(*line))
	{
		if (*line == '\'' || *line == '"')
		{
			quote = *line++;
			while (*line && *line != quote)
				line++;
			if (*line == quote)
				line++;
		}
		else
			line++;
	}
	return (line);
}
