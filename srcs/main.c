/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:04:20 by tiago             #+#    #+#             */
/*   Updated: 2022/10/22 13:45:47 by mimarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_data	data;

int	main(int argc, char **argv, char **envp)
{
	int	i;

	(void)argc;
	(void)argv;
	//signal handler CTRL+C CTRL+D CTRL+/
	setting_signal();
	//signal handler CTRL+C
	signal(SIGINT, handler);
	while (1)
	{
		//get the evironment pointer
		data.env = *envp;
		//get current working directory
		//Not working?
		data.pwd = getcwd(NULL, 0);
		//current folder
		data.rel_pwd = ft_strrchr(data.pwd, '/');
		data.rel_pwd++;

		//read the line and return a buffer with what was written (/!\needs free)
		data.buffer = readline(GREEN "minishell" RESET " " YELLOW "~" RESET " ");
		//if any input place the string at the end of the history list.
		////Não entendo o add_history
		if (ft_strlen(data.buffer) > 0)
			add_history(data.buffer);
		//split commands on semi-column
		data.cmds = ft_split(data.buffer, ';');
		// cont the commands
		data.cmd_count = ft_count(data.cmds);
		//if there are commands
		if (data.cmd_count > 0)
		{
			//if command == exit -> GTFO
			if (strcmp(data.buffer, "exit") == 0)
				break ;
				
			//print data struct
			printf("cmd_count: %d\n", data.cmd_count);
			printf("pwd: %s\n", data.pwd);
			printf("rel_pwd: %s\n", data.rel_pwd);
			printf("buffer: %s\n\n", data.buffer);

			i = 0;
			//print commands
			while (i < data.cmd_count)
			{
				printf("cmds[%d]: %s\n", i, data.cmds[i]);
				i++;
			}
			if (data.buffer[0] != '\0')
				printf("%s\n", data.buffer);
			//if clear; clear console
			if (strcmp(data.buffer, "clear") == 0)
				system("clear");
		}
	}
	return (0);
}
