#include "../include/minishell.h"

void    handle_sigint(int sig)
{
    (void)sig;
    rl_replace_line("", 0);
    write(1, "\n", 1);
    rl_on_new_line();
    rl_redisplay();
}

void    handle_sigquit(int sig)
{
    (void)sig;
    write(1, "\b\b  \b\b", 6);
}

void    setup_signals()
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
}
