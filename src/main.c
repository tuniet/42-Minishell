#include "../include/minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_data data;
	if(argc > 1 || argv[1] != NULL)
		return (printf("Usage: %s\n", argv[0]), 1);
	if(!init_data(&data, envp))
		return (1);

	setup_signals();

	using_history();
	read_history(".minishell_history");

	char	*line;
	while (1)
	{
		if (get_prompt(&data.prompt, &data) == 0)
			return (0);
		line = readline(data.prompt);
		free(data.prompt);
		data.prompt = NULL;
		if (!line)
		{
			write_history(".minishell_history");
			return (printf("exit\n"), 0);
		}
		if (line[0])
			add_history(line);

		//PROVISIONAL
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
		else{
			parse_line(line);
			free(line);
		}
	}
	return (0);
}
