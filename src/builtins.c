/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:15:20 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:15:23 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_arg(char *arg)
{
	size_t	len;

	len = ft_strlen(arg);
	if ((arg[0] == '\'' && arg[len - 1] == '\'')
		|| (arg[0] == '"' && arg[len - 1] == '"'))
	{
		arg[len - 1] = '\0';
		arg++;
	}
	printf("%s", arg);
}

int	mini_echo(char **argv)
{
	int	i;
	int	newline_;

	i = 1;
	newline_ = 1;
	if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
	{
		newline_ = 0;
		i = 2;
	}
	while (argv[i])
	{
		print_arg(argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline_)
		printf("\n");
	return (0);
}

int	mini_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}
