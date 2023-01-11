/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:24:21 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/11 14:21:14 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_pipeline(t_pipeline *pipeline)
{
	int i;

	i = 0;
	while (i < pipeline->num_cmds)
	{
		ft_free_cmd(pipeline->cmds[i]);
		i++;
	}
	free(pipeline->cmds);
	free(pipeline);
}

void	close_pipes(int num_pipes, int (*pipes)[2])
{
	int i;

	i = 0;
	while (i < num_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	execute_redir(t_command *cmd, int num_pipes, int (*pipes)[2])
{
	if (cmd->redirect[0] != -1)
		dup2(cmd->redirect[0], STDIN_FILENO);
	if (cmd->redirect[1] != -1)
		dup2(cmd->redirect[1], STDOUT_FILENO);
	close_pipes(num_pipes, pipes);
	return (0);
}

pid_t	run_with_redir(t_command *cmd, int num_pipes, int (*pipes)[2])
{
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid)
	{
		if (child_pid == -1)
		{
			printf("Error: fork failed\n");
			return (-1);
		}
		printf("Child pid: %d\n", child_pid);
		return (child_pid);
	}
	else
	{
		execute_redir(cmd, num_pipes, pipes);
		perror("Error: execve failed");
		return (0);
	}
}