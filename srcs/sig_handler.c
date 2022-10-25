/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 16:49:40 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/25 16:03:34 by tpereira         ###   ########.fr       */
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

	pid = waitpid(-1, &status, 0);
	if (signal == SIGINT)
	{
		if (pid == -1)
		{
			cwd = ft_relative_path(getcwd(NULL, 0));
			printf("%s➜%s %s%s%s ", BLUE, RESET, GREEN, cwd, RESET);
			rl_replace_line("\n", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else
			write(1, "\n", 1);
	}
	if (signal == SIGQUIT)
		write(1, "\n", 1);
}
