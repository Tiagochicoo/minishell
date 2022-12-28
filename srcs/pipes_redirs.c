/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 12:24:21 by tpereira          #+#    #+#             */
/*   Updated: 2022/12/28 12:25:21 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int pipes_redirs(void)
{
	int num_commands = 3;
	char *commands[3][3] = {
		{"ls", "-la", NULL},
		{"grep", ".c", NULL},
		{"wc", "-l", NULL}
	};
	char *envp[] = {NULL};
	char *output_file = "test.txt";

	int i;
	int prev_pipe[2] = {-1, -1};
	for (i = 0; i < num_commands; i++) {
		int pipefd[2];
		pipe(pipefd);

		pid_t pid = fork();
		if (pid == 0) {
			// Child process
			if (i > 0) {
				// If this is not the first command, redirect standard input to the read end of the previous pipe
				dup2(prev_pipe[0], STDIN_FILENO);
			}
			if (i < num_commands - 1) {
				// If this is not the last command, redirect standard output to the write end of the current pipe
				dup2(pipefd[1], STDOUT_FILENO);
			}
			// Close both ends of the pipes
			close(prev_pipe[0]);
			close(prev_pipe[1]);
			close(pipefd[0]);
			close(pipefd[1]);
			// Execute the command
			execve(commands[i][0], commands[i], envp);
		} else {
			// Parent process
			// Close both ends of the pipes
			close(prev_pipe[0]);
			close(prev_pipe[1]);
			close(pipefd[1]);
			// Wait for the child process to finish
			waitpid(pid, NULL, 0);
			// Save the read end of the current pipe for the next iteration
			prev_pipe[0] = pipefd[0];
		}
	}

	// Open the output file for appending
	int output_fd = open(output_file, O_WRONLY | O_APPEND | O_CREAT, 0666);
	if (output_fd < 0) {
		// Handle error
	}
	// Redirect standard input to the read end of the previous pipe
	dup2(prev_pipe[0], STDIN_FILENO);
	// Redirect standard output to the output file
	dup2(output_fd, STDOUT_FILENO);
	// Close both ends of the pipe
	close(prev_pipe[0]);
	// Execute the command
	execve(commands[num_commands - 1][0], commands[num_commands - 1], envp);

	return 0;
}
