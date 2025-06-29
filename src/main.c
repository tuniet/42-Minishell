#include "../include/minishell.h"

int main(int argc, char **argv, char **envp)
{
	t_data data;
	if(argc > 1 || argv[1] != NULL)
		return (printf("Usage: %s\n", argv[0]), 1);
	if(!init_data(&data, envp))
		return (1);

	char	*line;
	while (1)
	{
		if (get_prompt(&data.prompt, &data) == 0)
			return (0);
		line = readline(data.prompt);
		free(data.prompt);
		data.prompt = NULL;
		if (!line)
			return (printf("exit\n"), 0);
		/*if (command)
			continue ;
		*/
		parse_line(line);
		free(line);
	}
	return (0);
}
