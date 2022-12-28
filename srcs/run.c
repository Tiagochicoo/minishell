/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:35:39 by tpereira          #+#    #+#             */
/*   Updated: 2022/12/28 09:48:40 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_pipes(int num_pipes, int (*pipes)[2])
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

// void	execute_redir(t_cmd	*cmd, int num_pipes, int (*pipes)[2])
// {
// 	int	fd;

// 	fd = -1;
// 	fd = cmd->redirect[0];
// 	if (fd != -1)
// 		dup2(fd, STDIN_FILENO);
// 	fd = cmd->redirect[1];
// 	if (fd != -1)
// 		dup2(fd, STDOUT_FILENO);
// 	close_pipes(num_pipes, pipes);
// 	run_sys_cmd(cmd, cmd->args[0], 0);
// }

// pid_t	run_redir(t_cmd *cmd, int num_pipes, int (*pipes)[2])
// {
// 	pid_t	child_pid;

// 	child_pid = fork();
// 	if (child_pid)
// 	{
// 		if (child_pid == -1)
// 		{
// 			perror("fork failed");
// 			return (-1);
// 		}
// 		return (child_pid);
// 	}
// 	else
// 	{
// 		execute_redir(cmd, num_pipes, pipes);
// 		perror("exec failed");
// 		return (0);
// 	}
// }