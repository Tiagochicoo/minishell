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

int	main(void)
{
	while (1)
	{
		printf("%s %s %sminishel%s$%s> %s", RED, "φ",GREEN, BLUE, YELLOW, RESET);
		data.buffer = malloc(sizeof(1));
		strcpy(data.buffer, readline(""));
		if (strcmp(data.buffer, "exit") == 0)
			break ;
		if (data.buffer[0] != '\0')
			printf("%s\n", data.buffer);
	}
	return (0);
}
