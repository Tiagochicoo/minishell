/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
<<<<<<< HEAD
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 16:49:40 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/22 13:32:50 by mimarque         ###   ########.fr       */
=======
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 16:49:40 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/25 16:03:34 by tpereira         ###   ########.fr       */
>>>>>>> mitest
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void setting_signal()
{
	signal(SIGINT, handler);  // CTRL + C
	signal(SIGTERM, handler); // CTRL + D
	signal(SIGQUIT, handler); // CTRL + /
}

//Não entendo as ultimas 3 linhas
void handler(int signal)
{
<<<<<<< HEAD
	if (signal != SIGINT)
		return;
	printf("\n");
	//Tell the update functions that we have moved onto a new (empty) line, usually after outputting a newline.
	rl_on_new_line();
	//Replace the contents of rl_line_buffer with text. The point and mark are preserved, if possible.
	//If clear_undo is non-zero, the undo list associated with the current line is cleared.
	rl_replace_line("", 1);
	//Change what's displayed on the screen to reflect the current contents of rl_line_buffer.
	rl_redisplay();
=======
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
>>>>>>> mitest
}
