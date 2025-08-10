#include "../include/minishell.h"

void free_redirects(t_redirect *redir)
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


void free_command(t_command *cmd)
{
	if (!cmd)
		return;
	//NOTE: we dont allocate new tokens for command argv
	//		we take the t_token pointers at data->tokens
	if (cmd->argv)
	{
		free(cmd->argv);
		cmd->argv = NULL;
	}
	free_redirects(cmd->redirects);
	free(cmd);
}

