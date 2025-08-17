#include "../include/minishell.h"

int mini_echo(char **argv)
{
    int i = 1;
    int newline_ = 1;
    if (argv[1] && strcmp(argv[1], "-n") == 0)
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

/*
int mini_export(char **argv, t_data *data)
{
    if (!argv[1])
    {
        write(2, "export: se requiere VARIABLE=VALOR\n", 35);
        return 1;
    }
    // Implementa la lógica para añadir/modificar variables en el entorno
    return 0;
}
*/

// unset sin opciones
/*
int mini_unset(char **argv, t_data *data)
{
    if (!argv[1])
    {
        fprintf(stderr, "unset: se requiere VARIABLE\n");
        return 1;
    }
    // Implementa la lógica para eliminar la variable del entorno
    if (unsetenv(argv[1]) != 0)
    {
        perror("unset");
        return 1;
    }
    return 0;
}
*/

// env sin opciones o argumentos
int mini_env(t_data *data, char **argv)
{
    if (argv[1])
    {
        fprintf(stderr, "env: no se permiten argumentos\n");
        return 1;
    }
    for (int i = 0; data->envp[i]; i++)
        printf("%s\n", data->envp[i]);
    return 0;
}

int mini_cd(char **argv, t_data *data)
{
    if (!argv[1] || argv[2])
    {
        write(2, "cd: solo se permite una ruta\n", 29);
        return 1;
    }
    if (chdir(argv[1]) != 0)
    {
        perror("cd");
        return 1;
    }
    data->pwd = getcwd(NULL, 0);
    return 0;
}

int mini_exit(char **argv)
{
    int status = 0;
    if (argv[1])
        status = atoi(argv[1]);
    printf("exit\n");
    exit(status);
}

int is_builtin(const char *cmd)
{
    if (!cmd)
        return 0;
    return (
        strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "cd") == 0 ||
        strcmp(cmd, "pwd") == 0 ||
        strcmp(cmd, "env") == 0 ||
        strcmp(cmd, "exit") == 0
        // Add more builtins here if needed
    );
}

int execute_builtin(char **argv, t_data *data)
{
    if (!argv || !argv[0])
        return 1;
    if (strcmp(argv[0], "echo") == 0)
        return mini_echo(argv);
    if (strcmp(argv[0], "cd") == 0)
        return mini_cd(argv, data);
    if (strcmp(argv[0], "pwd") == 0)
        return mini_pwd();
    if (strcmp(argv[0], "env") == 0)
        return mini_env(data, argv);
    if (strcmp(argv[0], "exit") == 0)
        mini_exit(argv);
    // Add more builtins here if needed
    return 1;
}
