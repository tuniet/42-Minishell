#include "../include/minishell.h"
#include <string.h>
#include <stdlib.h>


int mini_echo(char **argv)
{
    int i = 1;
    int newline_ = 1;
    if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
    {
        newline_ = 0;
        i = 2;
    }
    while (argv[i])
    {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (newline_)
        printf("\n");
    return 0;
}

int mini_pwd(void)
{
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)))
    {
        printf("%s\n", cwd);
        return 0;
    }
    perror("pwd");
    return 1;
}
