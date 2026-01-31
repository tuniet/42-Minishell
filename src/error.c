/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 14:32:32 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/20 14:32:33 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_echo_error(const char *filename, const char *arg)
{
	write(STDERR_FILENO, filename, ft_strlen(filename));
	write(STDERR_FILENO, ": ", 3);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, "\n", 2);
}

void	print_export_error(const char *arg)
{
	write(STDERR_FILENO, "export: `", 10);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, "': not a valid identifier\n", 27);
}

void	error_exit(const char *cmd, const char *msg, int i_exit)
{
	if (cmd)
	{
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, ": ", 3);
	}
	if (msg)
		write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 2);
	exit(i_exit);
}
