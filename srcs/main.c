/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:04:20 by tiago             #+#    #+#             */
/*   Updated: 2022/09/22 18:10:06 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

char buff[500];

int	main(void)
{
	while (1)
	{
		printf("minishell$ > ");
		//scanf(buff);
		gets(buff);
		if (strcmp(buff, "exit") == 0)
			break ;
		if (buff[0] != '\0')
			puts(buff);
	}
	return (0);
}
