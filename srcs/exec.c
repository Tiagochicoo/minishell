/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdias-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/14 15:31:36 by jdias-mo          #+#    #+#             */
/*   Updated: 2022/11/15 17:43:17 by jdias-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	n_pipe(t_command *cmd)//conta pipes
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (i < ft_lstsize(cmd->args))
	{
		if ((cmd->args)->content.tok_type == 'PIPE')
			n++;
		cmd->args = (cmd->args)->next;
	}
	return (n);
}

void	has_redirects(t_command *cmd) //adicionar redirects ao t_cmd input e output. falta funçao para executar redirect
{
	int	i;

	i = 0
	while (i < ft_lstsize(cmd->args))
	{
		if ((cmd->args)->content.tok_type == 'INPUT')
			cmd->input = (cmd->args)->content->token;
		else if ((cmd->args)->content.tok_type == 'OUTPUT' || (cmd->args)->content.tok_type == 'APPEND')
			cmd->output = (cmd->args)->content->token;
		i++;
		cmd->args = (cmd->args)->next;
	}
}

void	parse_pipe(t_command *cmd) //adicionar comando e argumentos ao argv. um por string. incompleto
{
	int		i;
	char	*tmp;

	i = 0;
	while (i < ft_lstsize(cmd->args))
	{
		if (((cmd->args)->next->content.tok_type == 'PIPE' && (cmd->args)->content->tok_type == 'TEXT') || (cmd->args)->content.tok_type == 'PIPE')
		{
			if (!cmd->argv[i])
				cmd->argv[i] = ft_strdup((cmd->args)->content->token);
			i++;
			cmd->args = (cmd->args)->next;
		}
	}
}

void	run_pipes(t_command *cmd)//cria process child e faz redirects e piping antes de executar
{
	int	pid;
	pid = fork();
	if(!cmd->pipe.pid)
	{
		if(pipe.i == 0)
		{
			if (cmd->input)
				dup2(input, 0);//redirect falta funçao
			dup2(cmd->pipe.fd[0][1], 1);
		}
		else if(cmd->pipe.i > 0 && cmd->pipe.i < cmd->pipe.n)
		{
			dup2(cmd->pipe.fd[cmd->pipe.i][0], 0);
			dup2(cmd->pipe.fd[cmd->pipe.i][1], 1);
		}
		else if(pipe.i == pipe.n)
		{
			if (cmd->output)
				dup2(y, 1);//redirect falta funçao
			dup2(cmd->pipe.fd[i][0], 0);
		}
		close_pipes(cmd);
		if (cmd->type == NONE)
			exec(cmd);
		else
			run_builtin_cmd(cmd);
	}
}

void	close_pipes(t_command *cmd)//fechar pipes
{
	int	i;

	i = 0;
	while (i < cmd->pipe.n)
	{
		close(cmd->pipe.fd[i][0]);
		close(cmd->pipe.fd[i][1]);
	}
}

void	run(t_command *cmd)//faz redirects e executa comando sem pipes
{
	int	pid;

	if(!pid = fork())
	{
		if (cmd->input)
			dup2(input, 0);//falta funçao redirects
		if (cmd->output)
			dup2(output, 1);
		if (cmd->type == NONE)
			exec(cmd);
		else
			run_builtin_cmd(cmd);
	}
}

void	exec(t_command *cmd)//executa comando e argumentos em argv
{
	pid_t childPid;
	char	*path;

	path = cmd->argv[0];
	if (execve(path, "grep", "me", cmd->envp) < 0)
	{
		printf("%sError: command not found: %s%s\n", RED, RESET, cmd->argv[0]);
		exit(0);
	}
	free(path);
}

void	executor(t_command *cmd)//verifica redirects e pipes. parse comandos pipe. cria pipes. trata dos pipes e executa ou executa tb se nao tiver pipes.
{
	has_redirects(cmd);
	cmd->pipe.i = 0;
	if ((cmd->pipe.n = has_pipe(cmd)))
	{
		parse_pipe(cmd);
		cmd->pipe->fd = malloc(sizeof(int *) * cmd->pipe->n);
		while (cmd->pipe.i < cmd->pipe.n)
		{
			cmd->pipe->fd[cmd->pipe.i] = malloc(sizeof(int) * 2);
			run_pipes(cmd);
			cmd->pipe.i++;
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

//faltam frees, closes?, error checks,...
