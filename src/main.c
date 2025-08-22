/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <antoniof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:27:16 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/22 17:14:39 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	compute_line(char *line, t_data *data)
{
	int	count;
	int	start_end[2];

	if (line == NULL || line[0] == '\0')
		return (0);
	count = tokenize(line, data->tokens);
	if (count <= 0)
		return (0);
	data->tokens_size = count;
	start_end[0] = 0;
	start_end[1] = count - 1;
	data->ast_root = build_tree(data->tokens, start_end, data);
	if (!data->ast_root)
		return (print_syntax_error(data), 0);
	execute_tree(data->ast_root, data->envp, data);
	return (1);
}

static int	handle_input(char *line, t_data *data)
{
	if (!compute_line(line, data))
	{
		free_all(data, 0);
		free(line);
		return (0);
	}
	free(line);
	free_all(data, 0);
	return (1);
}

int	main_loop(t_data *data)
{
	char	*line;

	line = NULL;
	while (1)
	{
		if (get_prompt(&data->prompt, data) == 0)
			break ;
		line = readline(data->prompt);
		mini_free((void **)&data->prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (line[0])
			add_history(line);
		handle_input(line, data);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	char	*shlvl;

	if (argc > 1)
		return (printf("Usage: %s\n", argv[0]), 1);
	if (!init_data(&data, envp))
		return (1);
	setup_signals();
	shlvl = ft_itoa(ft_atoi(mini_getenv("SHLVL", data.envp)) + 1);
	set_env_var(&data.envp, "SHLVL", shlvl);
	free(shlvl);
	main_loop(&data);
	shlvl = ft_itoa(ft_atoi(mini_getenv("SHLVL", data.envp)) - 1);
	set_env_var(&data.envp, "SHLVL", shlvl);
	free(shlvl);
	return (free_all(&data, 1), data.i_exit);
}
