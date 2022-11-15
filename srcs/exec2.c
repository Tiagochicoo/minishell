/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdias-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:31:36 by jdias-mo          #+#    #+#             */
/*   Updated: 2022/11/15 15:40:53 by jdias-mo         ###   ########.fr       */
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
		if ((cmd->args)->tok_type == 'PIPE')
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

void	run_pipes(t_command *cmd)
{
	cmd->pipe.pid = fork();
	if(!cmd->pipe.pid)
	{
		if(pipe.i == 0)
		{
			if (cmd->input)
				dup2(input, 0);
			dup2(cmd->pipe.fd[0][1], 1);
		}
		else if(cmd->pipe.i > 0 && cmd->pipe.i < cmd->pipe.n)
		{
			dup2(cmd->pipe.fd[i][0], 0);
			dup2(cmd->pipe.fd[i][1], 1);
		}
		else if(pipe.i == pipe.n)
		{
			if (cmd->output)
				dup2(y, 1);
			dup2(cmd->pipe.fd[i][0], 0);
		}
		close_pipes(cmd);
		if (cmd->type == NONE)
			exec();
		else
			run_builtin_cmd(cmd);
	}
}

void	close_pipes(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->pipe.n)
	{
		close(cmd->pipe.fd[i][0]);
		close(cmd->pipe.fd[i][1]);
	}
}

void	run(t_command *cmd)
{
	int	pid;

	if(!pid = fork())
	{
		if (cmd->input)
			dup2(input, 0);
		if (cmd->output)
			dup2(output, 1);
		if (cmd->type == NONE)
			exec();
		else
			run_builtin_cmd(cmd);
	}

}

void	create_pipes(t_command *cmd)
{
	int	i;

	i = 0;
	if ((cmd->pipe.n = has_pipe(cmd)))
	{
		parse_pipe(cmd);
		while (i < cmd->pipe.n)
		{
			fd[pipe.i][2];
			run_pipes(cmd);
		}
		close_pipes(cmd);
		waitpid(-1, NULL, 0);
	}
	else
	{
		run(cmd);
		waitpid(-1, NULL, 0);
	}
}
