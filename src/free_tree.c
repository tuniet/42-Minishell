/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:17:23 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:17:27 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_redirects(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->filename);
		redir->filename = NULL;
		free(redir);
		redir = tmp;
	}
}

void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->argv)
	{
		free(cmd->argv);
		cmd->argv = NULL;
	}
	free_redirects(cmd->redirects);
	free(cmd);
}
