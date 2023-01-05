/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 16:49:40 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/04 17:58:23 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void setting_signal()
{
	signal(SIGINT, handler);  // CTRL + C
	signal(SIGTERM, handler); // CTRL + D
	signal(SIGQUIT, handler); // CTRL + /
}

void handler(int signal)
{
	pid_t	pid;
	int		status;
	char	*cwd;
	char	*tmp;

	pid = waitpid(-1, &status, 0);
	tmp = getcwd(NULL, 0);
	if (signal == SIGINT)
	{
		if (pid == -1)
		{
			cwd = ft_relative_path(tmp);
			printf("\n%s➜%s %s%s%s ", BLUE, RESET, GREEN, cwd, RESET);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			free(cwd);
		}
		else
			write(1, "\n", 1);
	}
	if (signal == SIGQUIT)
		write(1, "\n", 1);
	free(tmp);	
}
