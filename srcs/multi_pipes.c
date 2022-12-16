/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 10:15:32 by tpereira          #+#    #+#             */
/*   Updated: 2022/12/16 10:37:43 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	int fd[3][2];
	int pid1;
	int pid2;
	int pid3;
	int i;
	int x;

	i = 0;
	printf("Enter a number: ");
	scanf("%d", &x);
	while (i < 3)
	{
		if (pipe(fd[i]) < 0)
			return (1);
		i++;
	}
	pid1 = fork();
	if (pid1 < 0)
		return (2);
	if (pid1 == 0)
	{
		close(fd[0][1]);
		close(fd[1][0]);
		close(fd[2][0]);
		close(fd[2][1]);
		if (read(fd[0][0], &x, sizeof(int)) < 0)
			return (3);
		x += 5;
		if (write(fd[1][1], &x, sizeof(int)) < 0)
			return (4);
		close(fd[0][0]);
		close(fd[1][1]);
		return (0);
	}

	pid2 = fork();
	if (pid2 < 0)
		return (5);
	if (pid2 == 0)
	{
		close(fd[0][0]);
		close(fd[0][1]);
		close(fd[1][1]);
		close(fd[2][0]);
		if (read(fd[1][0], &x, sizeof(int)) < 0)
			return (6);
		x += 5;
		if (write(fd[2][1], &x, sizeof(int)) < 0)
			return (7);
		close(fd[1][0]);
		close(fd[2][1]);
		return (0);
	}

	close(fd[0][0]);
	close(fd[1][0]);
	close(fd[1][1]);
	close(fd[2][1]);
	if (write(fd[0][1], &x, sizeof(int)) < 0)
		return (8);
	if (read(fd[2][0], &x, sizeof(int)) < 0)
		return (9);
	printf("Result is %d\n", x);
	close(fd[0][1]);
	close(fd[2][0]);

	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	return (0);
}