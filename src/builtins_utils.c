/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:15:30 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:15:33 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	mini_env(t_data *data, char **argv)
{
	int	i;

	i = 0;
	if (argv[1])
	{
		fprintf(stderr, "env: no se permiten argumentos\n");
		return (1);
	}
	while (data->envp[i])
		printf("%s\n", data->envp[i++]);
	return (0);
}

int	mini_cd(char **argv, t_data *data)
{
	if (!argv[1] || argv[2])
	{
		write(2, "cd: solo se permite una ruta\n", 29);
		return (1);
	}
	if (chdir(argv[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	data->pwd = getcwd(NULL, 0);
	return (0);
}

int	mini_exit(char **argv)
{
	int	status;

	status = 0;
	if (argv[1])
		status = atoi(argv[1]);
	printf("exit\n");
	free_argv(argv);
	exit(status);
}

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0);
}

int	execute_builtin(char **argv, t_data *data)
{
	int	ret;

	if (!argv || !argv[0])
		ret = 1;
	if (ft_strcmp(argv[0], "echo") == 0)
		ret = mini_echo(argv);
	if (ft_strcmp(argv[0], "cd") == 0)
		ret = mini_cd(argv, data);
	if (ft_strcmp(argv[0], "pwd") == 0)
		ret = mini_pwd();
	if (ft_strcmp(argv[0], "env") == 0)
		ret = mini_env(data, argv);
	if (ft_strcmp(argv[0], "exit") == 0)
		mini_exit(argv);
	if (ft_strcmp(argv[0], "export") == 0)
		ret = mini_export(argv, data);
	if (ft_strcmp(argv[0], "unset") == 0)
		ret = mini_unset(argv, data);
	free_argv(argv);
	return (ret);
}
