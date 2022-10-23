/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:04:20 by tiago             #+#    #+#             */
/*   Updated: 2022/10/23 15:33:32 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_data	data;

int	main(int argc, char **argv, char **envp)
{
	// int	i;
	pid_t	child;

	(void)argc;
	(void)argv;
	signal(SIGINT, handler);
	while (1)
	{
		data.env = *envp;
		data.pwd = getcwd(NULL, 0);
		data.rel_pwd = ft_strrchr(data.pwd, '/');
		data.rel_pwd++;
		
		data.buffer = readline(GREEN "minishell" RESET " " YELLOW "~" RESET " ");
		if (ft_strlen(data.buffer) > 0)
			add_history(data.buffer);
		data.cmds = ft_split(data.buffer, ' ');
		data.cmd_count = ft_count(data.cmds);
		if (data.cmd_count > 0)
		{
			if (strcmp(data.buffer, "exit") == 0)
				break ;
				
			// printf("cmd_count: %d\n", data.cmd_count);
			// printf("pwd: %s\n", data.pwd);
			// printf("rel_pwd: %s\n", data.rel_pwd);
			// printf("buffer: %s\n\n", data.buffer);
	
			// i = 0;
			// while (i < data.cmd_count)
			// {
			// 	printf("cmds[%d]: %s\n", i, data.cmds[i]);
			// 	i++;
			// }
			// if (data.buffer[0] != '\0')
			// 	printf("%s\n", data.buffer);
			else if (strcmp(data.buffer, "clear") == 0)
				system("clear");
			else if (strcmp(data.buffer, "ls") == 0)
			{
				child = fork();
				if (child == 0)
					execve("/bin/ls", data.cmds, &data.env);
				else
					wait(NULL);
			}
		}
	}
	return (0);
}
