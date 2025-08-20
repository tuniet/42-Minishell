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

static int	check_final_pattern(const char *p)
{
	while (*p == '*')
		p++;
	if (*p == 0)
		return (0);
	return (1);
}

static int	wildcard_match(const char *pattern, const char *string)
{
	const char	*star_str_pat[2];

	star_str_pat[0] = NULL;
	star_str_pat[1] = NULL;
	while (*string)
	{
		if (*pattern == '*')
		{
			star_str_pat[0] = pattern++;
			star_str_pat[1] = string;
		}
		else if (*pattern == '?' || *pattern == *string)
		{
			pattern++;
			string++;
		}
		else if (star_str_pat[0])
		{
			pattern = star_str_pat[0] + 1;
			string = ++star_str_pat[1];
		}
		else
			return (1);
	}
	return (check_final_pattern(pattern));
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

static char	**process_directory_entries(const char *pattern, DIR *dir)
{
	struct dirent	*ent;
	char			**matches;
	int				count;

	matches = NULL;
	count = 0;
	ent = readdir(dir);
	while (ent != NULL)
	{
		if (wildcard_match(pattern, ent->d_name) == 0)
		{
			matches = realloc_matches(matches, count + 2);
			if (!matches)
				return (NULL);
			matches[count] = strdup(ent->d_name);
			if (!matches[count])
				return (NULL);
			count++;
			matches[count] = NULL;
		}
		ent = readdir(dir);
	}
	if (count == 0)
		return (ft_split(pattern, ' '));
	return (matches);
}

char	**expand_wildcards(const char *pattern)
{
	DIR		*dir;
	char	**result;

	dir = opendir(".");
	if (!dir)
		return (create_no_match_result(pattern));
	result = process_directory_entries(pattern, dir);
	closedir(dir);
	return (result);
}
