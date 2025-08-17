/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoniof <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:26:34 by antoniof          #+#    #+#             */
/*   Updated: 2025/08/17 20:26:35 by antoniof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	make_pwd(t_data *data, char *prompt)
{
	if (data->home && ft_strncmp(data->pwd, data->home,
			ft_strlen(data->home)) == 0)
	{
		ft_strcat(prompt, "~");
		ft_strcat(prompt, data->pwd + ft_strlen(data->home));
	}
	else
	{
		ft_strcat(prompt, data->pwd);
	}
}

static int	print_prompt(char *prompt, char *user, char *host, t_data *data)
{
	ft_strcpy(prompt, BRIGHT_GREEN);
	ft_strcat(prompt, user);
	ft_strcat(prompt, "@");
	ft_strcat(prompt, host);
	ft_strcat(prompt, RESET_COLOR);
	ft_strcat(prompt, ":" BRIGHT_BLUE);
	make_pwd(data, prompt);
	ft_strcat(prompt, RESET_COLOR "$ ");
	return (1);
}
