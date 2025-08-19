/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils2.c                                       :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:19:07 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 18:19:09 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	add_argument(t_command *cmd, t_token *arg)
{
	t_token	**new_argv;
	size_t	new_capacity;

	if (cmd->argc + 1 >= cmd->capacity)
	{
		new_capacity = cmd->capacity * 2;
		new_argv = realloc(cmd->argv, sizeof(t_token *) * new_capacity);
		if (!new_argv)
			return (0);
		cmd->argv = new_argv;
		cmd->capacity = new_capacity;
	}
	cmd->argv[cmd->argc] = arg;
	cmd->argc++;
	cmd->argv[cmd->argc] = NULL;
	return (1);
}

t_redirect	*new_redirect(t_node_type type, char *filename, t_data *data)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->next = NULL;
	if (type == TOKEN_HEREDOC)
  {
    redir->filename = NULL;
		redir->hered_fd = heredoc(expand_token_(filename, data->envp, data->i_exit), data);
    if (redir->hered_fd < 0)
      return (free(redir), NULL);
  }
	else
  {
    redir->filename = ft_strdup(filename);
    if (!redir->filename) 
      return (free(redir), NULL);
		redir->hered_fd = -1;
  }
	return (redir);
}

int	add_redirection(t_command *cmd, t_node_type type,
				char *filename, t_data *data)
{
	t_redirect	*redir;
	t_redirect	*tmp;

	redir = new_redirect(type, filename, data);
	if (!redir)
		return (0);
	if (!cmd->redirects)
		cmd->redirects = redir;
	else
	{
		tmp = cmd->redirects;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
	}
	return (1);
}
