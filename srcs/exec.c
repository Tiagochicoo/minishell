/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdias-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 12:27:17 by jdias-mo          #+#    #+#             */
/*   Updated: 2022/11/11 01:17:24 by jdias-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_cmd(t_command *cmd)
{
	//look for pipes

	//look for redirects
}

void	exec_pipes(t_command *cmd)
{
	int	i;

	cmd->pipe.fd = malloc(sizeof(int) * 2 * cmd->pipe.n);//malloc pipe
	i = 0;
	while (i < cmd->pipe.n) //create pipes
	{
		if (pipe(cmd->pipe.fd + 2 * i) == -1)
			return ; //free
		i++;
	}
	cmd->pipe.i = -1;
	while (++cmd->pipe.i < cmd->pipe.n)//while pipes func fork
		child(cmd);
	i = -1;
	while (++i < cmd->pipe.n)//closes
		close(cmd->pipe.fd);
	waitpid(-1, NULL, 0);
	//frees
}

void	_dup2(int in, int out)
{
	dup2(in, 0);
	dup2(out, 1);
}

void	child(t_command *cmd)
{
	pid_t pid;

	pid = fork();
	if (!pid)
	{
		if (cmd->pipe.i == 0)							// primeiro pipe
			_dup2(0, cmd->pipe.fd[1]);
		else if (cmd->pipe.i == cmd->pipe.n - 1)		// ultimo pipe
			_dup2(cmd->pipe.fd[cmd->pipe.i * 2 - 2], 1);
		else											// pipes do meio
			_dup2(cmd->pipe.fd[cmd->pipe.i * 2 - 2], cmd->pipe.fd[cmd->pipe.i * 2 - 1]);
		execve(ft_findcmd(cmd), cmd->argv, cmd->envp);	// tem q avançar no comando e args com o index dos pipes.
	}
}
//faltam frees, error checks e raciocionio. o comando a executar e pipes tem que avançar com os pipes.
//t_command por cada pipe? com variavel na struct para pipe?
//ou ir buscar comandos e argumentos entre cada pipe à LL
