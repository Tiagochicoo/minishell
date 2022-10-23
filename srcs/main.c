/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 16:01:56 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/23 18:39:57 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

enum builtin_t parseBuiltin(struct command *cmd) {
	if (!strcmp(cmd->argv[0], "exit"))			// exit command
		return EXIT;
	else if (!strcmp(cmd->argv[0], "jobs")) 	// jobs command
		return JOBS;
	else if (!strcmp(cmd->argv[0], "bg"))		// bg command
		return BG;
	else if (!strcmp(cmd->argv[0], "fg"))		// fg command
		return FG;
	else
		return NONE;
}

void error(char *msg) {
	printf("Error: %s", msg);
	exit(0);
}

int parse(const char *input, struct command *cmd) {
	static char	array[MAXLINE];					// local copy of command line
	const char	delims[10] = " \t\r\n";			// argument delimiters
	char		*line;							// ptr that traverses command line
	char		*token;							// ptr to the end of the current arg
	char		*endline;						// ptr to the end of the input string
	int			is_bg;							// background job?

	line = array;
	if (input == NULL)
		error("command line is NULL\n");
	(void) ft_strncpy(line, input, MAXLINE);
	endline = line + ft_strlen(line);
	cmd->argc = 0;								// build argv list
	while (line < endline) 
	{
		line += strspn(line, delims);			// skip delimiters
		if (line >= endline)
			break;
		token = line + strcspn(line, delims);	// Find token delimiter
		*token = '\0';							// terminate the token
		cmd->argv[cmd->argc++] = line;			// Record token as the token argument
		if (cmd->argc >= MAXARGS-1) 			// Check if argv is full
			break;
		line = token + 1;
	}
	cmd->argv[cmd->argc] = NULL; 							// argument list must end with a NULL pointer
	if (cmd->argc == 0)										// ignore blank line
		return 1;
	cmd->builtin = parseBuiltin(cmd);
	if ((is_bg = (*cmd->argv[cmd->argc-1] == '&')) != 0) 	// should job run in background?
		cmd->argv[--cmd->argc] = NULL;

	return is_bg;
}

void run_sys_cmd(struct command *cmd, int bg)
{
	pid_t childPid;
	
	if ((childPid = fork()) < 0)					// fork a child process	
		error("fork() error");
	else if (childPid == 0)							// I'm the child and could run a command
	{
		if (execvp(cmd->argv[0], cmd->argv) < 0)	// EXECVE !!!!
		{
			printf("minishell: command not found: %s%s%s\n", RED, cmd->argv[0], RESET);
			exit(0);
		}
	}
	else	// I'm the parent. Shell continues here.
	{
		 if (bg) 
			printf("Child in background [%d]\n",childPid);
		 else
			wait(&childPid);
	}
}

void run_builtin_cmd(struct command *cmd) 
{
	switch (cmd->builtin) {
		case EXIT:
			exit(0);
		case JOBS:
			printf("TODO: jobs\n"); break;
		case BG:
			printf("TODO: background\n"); break;
		case FG:
			printf("TODO: foreground\n"); break;
		default:
			error("Unknown builtin command");
	}
}

void eval(char *input) 
{
	int				background;				// should job run in background?
	struct command	cmd;					// parsed command

	background = parse(input, &cmd);	 	// parse line into command struct
	if (background == -1)					// parse error
		return;
	if (cmd.argv[0] == NULL)				// empty line - ignore
		return;
	if (cmd.builtin == NONE)
		run_sys_cmd(&cmd, background);
	else 
		run_builtin_cmd(&cmd);
}

int main(int argc, char **argv) 
{
	char *input; 							// buffer for readline

	if (argv[0] != NULL)
	{
		while (argc)
		{
			input = readline(GREEN "minishell" RESET " " YELLOW "~" RESET " ");
			if (ft_strcmp(input, "\n") > 0)
				add_history(input);
			eval(input);					// Evaluate input
		}
	}
}