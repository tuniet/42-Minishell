/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:05:08 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 21:05:10 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <dirent.h>

// Auxiliary function that mimics fnmatch behavior for basic wildcard matching
static int	wildcard_match(const char *pattern, const char *string)
{
	const char	*p = pattern;
	const char	*s = string;
	const char	*star_pattern = NULL;
	const char	*star_string = NULL;

	while (*s)
	{
		if (*p == '*')
		{
			star_pattern = p++;
			star_string = s;
		}
		else if (*p == '?' || *p == *s)
		{
			p++;
			s++;
		}
		else if (star_pattern)
		{
			p = star_pattern + 1;
			s = ++star_string;
		}
		else
		{
			return (1); // No match
		}
	}
	while (*p == '*')
		p++;
	return (*p == 0 ? 0 : 1);
}

static char	**realloc_matches(char **matches, int new_size)
{
	char	**new_matches;
	int		i;

	new_matches = malloc(sizeof(char *) * new_size);
	if (!new_matches)
		return (NULL);
	i = 0;
	if (matches)
	{
		while (i < new_size - 2)
		{
			new_matches[i] = matches[i];
			i++;
		}
		free(matches);
	}
	return (new_matches);
}

char	**expand_wildcards(const char *pattern)
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
	ent = readdir(dir);
	while (ent != NULL)
	{
		if (wildcard_match(pattern, ent->d_name) == 0)
		{
			matches = realloc_matches(matches, count + 2);
			if (!matches)
			{
				closedir(dir);
				return (NULL);
			}
			matches[count] = strdup(ent->d_name);
			if (!matches[count])
			{
				closedir(dir);
				return (NULL);
			}
			count++;
			matches[count] = NULL;
		}
		ent = readdir(dir);
	}
	closedir(dir);
	if (count == 0)
		return (ft_split(pattern, ' '));
	return (matches);
}