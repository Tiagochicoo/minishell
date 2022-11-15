/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdias-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:31:36 by jdias-mo          #+#    #+#             */
/*   Updated: 2022/11/15 13:43:53 by jdias-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	n_pipe(t_command *cmd)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (i < ft_lstsize(cmd->args))
	{
		if (cmd->args->content == 'PIPE')
			n++;
		cmd->args = cmd->args->next;
	}
	return (n);
}

void	has_redirect(t_command *cmd)
{
	int	i;

	i = 0
	while (i < ft_lstsize(cmd->args))
	{
		if (cmd->args->content == '<')
			cmd->input = cmd->args->next->content;
		else if (cmd->args->content == '>' || cmd->args->content == '>>')
			cmd->output = cmd->args->next->content;
		i++;
	}
}

void	parse_pipe(t_command *cmd)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < ft_lstsize(cmd->args))
	{
		if (cmd->args->token == 'PIPE')
		if (!cmd->argv[j])
		cmd->argv[i] = ft_strdup(cmd->args->content)
	}
}

void	pipe(t_command *cmd)
{
	has_redirect();
	if ((cmd->pipe.n = has_pipe(cmd)))
	{
		parse_pipe(cmd);
	}
}
