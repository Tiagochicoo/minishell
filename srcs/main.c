/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:04:20 by tiago             #+#    #+#             */
/*   Updated: 2022/09/28 21:49:58 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_data	data;

int	main(int argc, char **argv, char **envp)
{
	int	i;

	while (argc)
	{
		data.argv2 = argv;
		data.env = *envp;
		data.pwd = getcwd(NULL, 0);
		data.rel_pwd = ft_strrchr(data.pwd, '/');
		data.rel_pwd++;
		
		data.buffer = readline(GREEN "minishell" RESET " " YELLOW "~" RESET " ");
		if (ft_strlen(data.buffer) > 0)
			add_history(data.buffer);
		data.cmds = ft_split(data.buffer, ';');
		data.cmd_count = ft_count(data.cmds);
		if (data.cmd_count > 0)
		{
			if (strcmp(data.buffer, "exit") == 0)
				break ;
				
			printf("cmd_count: %d\n", data.cmd_count);
			printf("pwd: %s\n", data.pwd);
			printf("rel_pwd: %s\n", data.rel_pwd);
			printf("buffer: %s\n\n", data.buffer);
	
			i = 0;
			while (i < data.cmd_count)
			{
				printf("cmds[%d]: %s\n", i, data.cmds[i]);
				i++;
			}
			if (data.buffer[0] != '\0')
				printf("%s\n", data.buffer);
			if (strcmp(data.buffer, "clear") == 0)
				system("clear");
		}
	}
	return (0);
}
