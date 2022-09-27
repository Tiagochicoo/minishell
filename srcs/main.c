/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:04:20 by tiago             #+#    #+#             */
/*   Updated: 2022/09/27 15:41:11 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char buff[500];

int	main(void)
{
	while (1)
	{
		printf("%s %s %sminishel%s$%s> %s", RED, "φ",GREEN, BLUE, YELLOW, RESET);
		strcpy(buff, readline(""));
		if (strcmp(buff, "exit") == 0)
			break ;
		if (buff[0] != '\0')
			printf("%s\n", buff);
	}
	return (0);
}
