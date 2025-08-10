#include "../include/minishell.h"

void	print_tokens(t_token **tokens, int count)
{
	for (int i = 0; i < count; i++)
		printf("[%d] Type: %d | Value: %s\n", i, tokens[i]->type, tokens[i]->content);
}

int	compute_line(char *line, t_data *data)
{
	int	count;

	if (line == NULL || line[0] == '\0')
		return (0);
	count = tokenize(line, data->tokens);
	if (count <= 0)
		return (0);
	data->tokens_size = count;
	printf("tokens_size = [%d]\n", data->tokens_size);
	print_tokens(data->tokens, data->tokens_size);
	data->ast_root = build_tree(data->tokens, 0, count - 1);
	if (!data->ast_root)
		return (0);
	print_tree(data->ast_root, 0);
	/*
	execute_tree(data->ast_root, data->envp);
	*/
	return (1);
}

int	main_loop(t_data *data)
{
	char	*line;

	line = NULL;
	while (1)
	{
		if (get_prompt(&data->prompt, data) == 0)
			break;
		line = readline(data->prompt);
		mini_free((void **)&data->prompt);
		if (!line)
		{
			printf("exit\n");
			break;
		}
		if (line[0])
			add_history(line);
		//TODO: exit is a built-in
		if (ft_strncmp(line, "exit", 4) == 0 && (line[4] == '\0' || line[4] == ' ' || line[4] == '\n'))
		{
			free(line);
			write_history(".minishell_history");
			return (printf("exit\n"), 0);
		}
		if (ft_strncmp(line, "clear", 5) == 0 && (line[5] == '\0' || line[5] == ' ' || line[5] == '\n'))
		{
			free(line);
			printf("\033[H\033[J");
			continue;
		}
		if (!compute_line(line, data))
		{
			printf("------> compute_line fails\n");
			free_all(data, 0);
			free(line);
			continue;
		}
		printf("------> compute_line success\n");
		free(line);
		free_all(data, 0);
	}
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_data	data;

	if(argc > 1 || argv[1] != NULL)
		return (printf("Usage: %s\n", argv[0]), 1);
	if(!init_data(&data, envp))
		return (1);
	setup_signals();
	main_loop(&data);
	//return (rl_clear_history(), free_all(&data), 0);
	//TODO : clear history
	return (free_all(&data, 1), 0);
}
/*
		if(ft_strncmp(line, "exit", 4) == 0 && (line[4] == '\0' || line[4] == ' ' || line[4] == '\n'))
		{
			free(line);
			write_history(".minishell_history");
			return (printf("exit\n"), 0);
		}
		if (ft_strncmp(line, "clear", 5) == 0 && (line[5] == '\0' || line[5] == ' ' || line[5] == '\n'))
		{
			free(line);
			printf("\033[H\033[J");
			continue;
		}
		else
		{
			parse_line(line, &data);
			free(line);
		}
	}
	return (0);
*/
