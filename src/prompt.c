/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:18:11 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:18:12 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*mini_getenv(char *var, char *envp[])
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] == var[j] && envp[i][j] != '=')
			j++;
		if (envp[i][j] == '=' && var[j] == '\0')
			return (envp[i] + j + 1);
		i++;
	}
	return (NULL);
}

static char	*host_trim(char *host)
{
	char	*trimmed;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = 0;
	while (host[i] != '\0' && host[i] != '/')
		i++;
	i++;
	j = i;
	while (host[j] != '\0' && host[j] != '.')
		j++;
	trimmed = malloc(sizeof(char) * (j - i + 1));
	if (!trimmed)
		return (NULL);
	while (host[i] && i < j)
		trimmed[k++] = host[i++];
	trimmed[k] = '\0';
	return (trimmed);
}

char	*get_host(char **envp, int *free_host)
{
	char	*host;

	host = mini_getenv("HOST", envp);
	if (!host)
		host = mini_getenv("HOSTNAME", envp);
	if (!host)
	{
		host = mini_getenv("SESSION_MANAGER", envp);
		if (host)
		{
			host = host_trim(host);
			if (!host)
				return (NULL);
			*free_host = 1;
		}
		if (!host)
		{
			host = "42madrid";
			*free_host = 0;
		}
	}
	return (host);
}

static void	make_pwd(t_data *data, char *prompt)
{
	if (data->home && ft_strncmp(data->pwd, data->home,
			ft_strlen(data->home)) == 0)
	{
		ft_strcat(prompt, "~");
		ft_strcat(prompt, data->pwd + ft_strlen(data->home));
	}
	else
	{
		ft_strcat(prompt, data->pwd);
	}
}

static int	print_prompt(char *prompt, char *user, char *host, t_data *data)
{
    ft_strcpy(prompt, BRIGHT_GREEN);
    ft_strcat(prompt, user);
    ft_strcat(prompt, "@");
    ft_strcat(prompt, host);
    ft_strcat(prompt, RESET_COLOR);
    ft_strcat(prompt, ":"
        BRIGHT_BLUE);
    make_pwd(data, prompt);
    ft_strcat(prompt, RESET_COLOR "$ ");
    return (1);
}

int	get_prompt(char **p, t_data *data)
{
	char	*user;
	char	*host;
	int		free_host;
	char	prompt[4096];

	data->home = mini_getenv("HOME", data->envp);
	user = mini_getenv("USER", data->envp);
	if (!user)
		user = "minishell";
	free_host = 0;
	host = get_host(data->envp, &free_host);
	if (host == NULL)
		return (0);
	print_prompt(prompt, user, host, data);
	*p = ft_strdup(prompt);
	if (free_host == 1)
		free(host);
	return (1);
}
