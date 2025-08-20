#include "../include/minishell.h"

void	print_export_error(const char *arg)
{
	write(2, "export: `", 9);
	write(2, arg, strlen(arg));
	write(2, "': not a valid identifier\n", 26);
}

