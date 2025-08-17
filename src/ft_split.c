#include "../include/minishell.h"

static int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*dup_word(const char *s, int start, int end)
{
	char	*word;
	int		len;

	len = end - start;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	memcpy(word, s + start, len);
	word[len] = '\0';
	return (word);
}

char	**ft_split(const char *s, char c)
{
	char	**arr;
	int		i;
	int		j;
	int		start;

	if (!s)
		return (NULL);
	arr = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (i > start)
		{
			arr[j] = dup_word(s, start, i);
			if (!arr[j])
				return (free_argv(arr), NULL);
			j++;
		}
	}
	arr[j] = NULL;
	return (arr);
}
