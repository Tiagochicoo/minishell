/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 16:49:40 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/22 13:32:50 by mimarque         ###   ########.fr       */
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
}
