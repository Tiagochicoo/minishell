/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:04:20 by tiago             #+#    #+#             */
/*   Updated: 2022/09/27 16:07:05 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_data	data;

int	main(int argc, char **argv, char **envp)
{
	int	i;

	i = 0;
	if (argc == 1)
	{
		data.argv2 = argv;
		data.env = *envp;
		data.pwd = getcwd(NULL, 0);
		data.rel_pwd = ft_strrchr(data.pwd, '/');
		data.rel_pwd++;
		data.buffer = readline(GREEN "minishell" RESET " " YELLOW "~" RESET " ");
		data.cmds = ft_split(data.buffer, ' ');
		data.cmd_count = ft_word_count(data.cmds[0], ' ');
		printf("cmd_count: %d\n", data.cmd_count);
		printf("pwd: %s\n", data.pwd);
		printf("rel_pwd: %s\n", data.rel_pwd);
		printf("buffer: %s\n", data.buffer);
		while (i < data.cmd_count && data.cmds != NULL && data.cmds[i] != NULL)
		{
			printf("cmds[%d]: %s\n", i, data.cmds[i]);
			i++;
		}
	}

	while (1)
	{
		if (ft_strlen(data.buffer) > 0)
		{
			add_history(data.buffer);
			free(data.buffer);
		}
		data.buffer = readline(GREEN "minishell" RESET " " YELLOW "~" RESET " ");
		if (strcmp(data.buffer, "exit") == 0)
			break ;
		if (data.buffer[0] != '\0')
			printf("%s\n", data.buffer);
		if (strcmp(data.buffer, "clear") == 0)
			system("clear");
	}
	return (0);
}
