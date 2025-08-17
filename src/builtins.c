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
        char *arg = argv[i];
        size_t len = ft_strlen(arg);
        if ((arg[0] == '\'' && arg[len - 1] == '\'') ||
            (arg[0] == '"' && arg[len - 1] == '"'))
        {
            arg[len - 1] = '\0';
            arg++;
        }
        printf("%s", arg);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (newline_)
        printf("\n");

    return (0);
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
