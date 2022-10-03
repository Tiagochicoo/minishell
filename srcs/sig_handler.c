/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 16:49:40 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/03 17:05:16 by tpereira         ###   ########.fr       */
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
	if (signal != SIGINT)
		return;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}
