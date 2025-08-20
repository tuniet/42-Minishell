/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:31:51 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 14:31:55 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	mini_env(t_data *data, char **argv)
{
	int	i;

	i = 0;
	if (argv[1])
	{
		write(2, "env: no se permiten argumentos\n", 32);
		return (1);
	}
	while (data->envp[i])
		printf("%s\n", data->envp[i++]);
	return (0);
}

int	mini_cd(char **argv, t_data *data)
{
	const char	*path;

	if (argv_len(argv) != 2)
	{
		write(2, "cd: too many arguments\n", 23);
		return (1);
	}
	if (!argv[1])
	{
		path = mini_getenv("HOME", data->envp);
		if (!path)
		{
			write(2, "cd: HOME no está definido\n", 26);
			return (1);
		}
	}
	else
		path = argv[1];
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	free(data->pwd);
	data->pwd = getcwd(NULL, 0);
	update_envp(data->envp, "PWD", data->pwd);
	return (0);
}

int	mini_exit(char **argv, t_data *data)
{
	long long	status;

	printf("exit\n");
	if (!argv[1])
		exit(data->i_exit);
	if (!is_numeric(argv[1]) || !atollong(argv[1], &status))
	{
		write(2, "exit: numeric argument required\n", 32);
		exit(2);
	}
	if (argv[2])
	{
		write(2, "exit: too many arguments\n", 25);
		return (1);
	}
	status = ft_atoi(argv[1]);
	exit((int)status);
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

int	execute_builtin(t_treenode *node, char **argv, t_data *data)
{
	int	ret;

	if (!argv || !argv[0])
		ret = 1;
	if (ft_strcmp(argv[0], "echo") == 0)
		ret = mini_echo(node, data, argv);
	if (ft_strcmp(argv[0], "cd") == 0)
		ret = mini_cd(argv, data);
	if (ft_strcmp(argv[0], "pwd") == 0)
		ret = mini_pwd();
	if (ft_strcmp(argv[0], "env") == 0)
		ret = mini_env(data, argv);
	if (ft_strcmp(argv[0], "exit") == 0)
		ret = mini_exit(argv, data);
	if (ft_strcmp(argv[0], "export") == 0)
		ret = mini_export(argv, data);
	if (ft_strcmp(argv[0], "unset") == 0)
		ret = mini_unset(argv, data);
	free_argv(argv);
	data->i_exit = ret;
	return (ret);
}
