/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:24:21 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/10 16:53:45 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// t_pipeline *parse_pipeline(char *input, char **envp)
// {
// 	char		*temp;
// 	char		*current;
// 	int			num_cmds;
// 	t_pipeline	*pipeline;

// 	num_cmds = 0;
// 	temp = ft_strdup(input);
// 	current = temp;
// 	while(*current)
// 	{
// 		if (*current == '|')
// 			num_cmds++;
// 		current++;
// 	}
// 	num_cmds++;
// 	pipeline = ft_calloc(sizeof(t_pipeline) + num_cmds * sizeof(t_command *), 1);
// 	pipeline->num_cmds = num_cmds;
// 	pipeline->envp = envp;
// 	return (pipeline);
// }

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
	printf("cmd->redirect[0] = %d\n", cmd->redirect[0]);
	printf("cmd->redirect[1] = %d\n", cmd->redirect[1]);
	if (cmd->redirect[0] != -1)
		dup2(cmd->redirect[0], STDIN_FILENO);
	if (cmd->redirect[1] != -1)
		dup2(cmd->redirect[1], STDOUT_FILENO);
	close_pipes(num_pipes, pipes);
	printf("pipes closed\n");
	return (file_exists(cmd));
}

pid_t	run_with_redir(t_command *cmd, int num_pipes, int (*pipes)[2])
{
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid)
	{
		if (child_pid == -1)
			return (-1);
		//execute_redir(cmd, num_pipes, pipes);
		return (child_pid);
	}
	else
	{
		execute_redir(cmd, num_pipes, pipes);
		perror("Error: execve failed");
		return (0);
	}
	return (child_pid);
}