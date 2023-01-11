/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 16:01:56 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/11 16:38:43 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_builtin parseBuiltin(t_command *cmd) 
{
	if (!strcmp(cmd->argv[0], "echo"))			// echo command
		return ECHO;
	else if (!strcmp(cmd->argv[0], "cd")) 		// cd command
		return CD;
	else if (!strcmp(cmd->argv[0], "pwd"))		// pwd command
		return PWD;
	else if (!strcmp(cmd->argv[0], "export"))	// export command
		return EXPORT;
	else if (!strcmp(cmd->argv[0], "env"))		// env command
		return ENV;
	else if (!strcmp(cmd->argv[0], "unset"))	// unset command
		return UNSET;
	if (!strcmp(cmd->argv[0], "exit"))			// exit command
		return EXIT;
	if (!strcmp(cmd->argv[0], "42"))			// 42 command
		return FT;
	else
		return NONE;
}

void error(char *msg) 
{
	printf("Error: %s", msg);
	exit(0);
}

// int parse(const char *input, t_command *cmd) 
// {
// 	static char	array[MAXLINE];								// local copy of command line
// 	const char	delims[10] = " \t\r\n\"";					// argument delimiters
// 	char		*line;										// ptr that traverses command line
// 	char		*token;										// ptr to the end of the current arg
// 	char		*endline;									// ptr to the end of the input string
// 	int			is_bg;										// background job?

// 	line = array;
// 	if (input == NULL)
// 		error("command line is NULL\n");
// 	(void) ft_strncpy(line, input, MAXLINE);
// 	endline = line + ft_strlen(line);
// 	cmd->argc = 0;											// build argv list
// 	while (line < endline)
// 	{
// 		line += ft_strspn(line, delims);					// skip delimiters
// 		if (line >= endline)
// 			break;
// 		token = line + ft_strcspn(line, delims);			// Find token delimiter
// 		*token = '\0';										// terminate the token
// 		strncpy(cmd->argv[cmd->argc++], line, ft_strlen(line));						// Record token as the token argument
// 		if (cmd->argc >= MAXARGS - 1)						// Check if argv is full
// 			break;
// 		line = token + 1;
// 	}
// 	//cmd->argv[cmd->argc] = NULL;							// argument list must end with a NULL pointer
// 	if (cmd->argc == 0)										// ignore blank line
// 		return 1;
// 	cmd->builtin = parseBuiltin(cmd);
// 	is_bg = (*cmd->argv[cmd->argc-1] == '&');
// 	if (is_bg)												// should job run in background?
// 		cmd->argv[--cmd->argc] = NULL;
// 	return is_bg;
// }

int	file_exists(t_command *cmd, pid_t child_pid)
{
	char	*tmp;

	tmp = ft_find_cmd(cmd);
	if (tmp != NULL)
	{
		free(tmp);
		run_sys_cmd(cmd, child_pid);
	}
	else if (!access(cmd->argv[0], F_OK))
	{
		free(tmp);
		if (!access(cmd->argv[0], X_OK))
			run_sys_cmd(cmd, child_pid);
		else
			perror("Error");
	}
	else
		free(tmp);
	return (0);
}

void	run_sys_cmd(t_command *cmd, pid_t child_pid)
{
	char	*path;

	path = ft_find_cmd(cmd);
	if (child_pid == 0)
		child_pid = fork();
	if (child_pid < 0)
		error("fork() error");
	else if (child_pid == 0)										// I'm the child and could run a command
	{
		if (cmd->builtin != NONE)
			run_builtin_cmd(cmd, child_pid);
		else if (execve(path, cmd->argv, cmd->envp) < 0)				// EXECVE != EXECVP
		{
			printf("%sError: command not found: %s%s\n", RED, RESET, cmd->argv[0]);
			exit(0);
		}
		free(path);
	}
	else														// I'm the parent. Shell continues here.
		wait(&child_pid);
}

pid_t	run_builtin_cmd(t_command *cmd, pid_t child_pid) 
{
	if (cmd->builtin == ECHO)
		echo(cmd);
	else if (cmd->builtin == CD)
		cd(cmd);
	else if (cmd->builtin == PWD)
		pwd();
	else if (cmd->builtin == EXPORT)
		export(cmd);
	else if (cmd->builtin == ENV)
		env(cmd->envp);
	else if (cmd->builtin == UNSET)
		unset(cmd);
	else if (cmd->builtin == EXIT)
		ft_exit(cmd);
	else if (cmd->builtin == FT)
		ft_ft();
	return (child_pid);
}

void	run(t_command *cmd)
{
	if (cmd->builtin != NONE)
		run_builtin_cmd(cmd, 0);
	else
		file_exists(cmd, 0);
	ft_free_cmd(cmd);
}

int	run_with_fork(t_command *cmd, int num_pipes, int (*pipes)[2])
{
	pid_t	child_pid;

	if (cmd->builtin != NONE)
		return (run_builtin_cmd(cmd, 0));
	child_pid = fork();
	if (child_pid)
	{
		if (child_pid == -1)
			return (-1);
		else if (cmd->builtin != NONE)
			return (run_builtin_cmd(cmd, child_pid));
		return (child_pid);
	}
	else
	{
		execute_redir(cmd, num_pipes, pipes);
		file_exists(cmd, child_pid);
		//run_sys_cmd(cmd, child_pid);
		return (0);
	}
	ft_free_cmd(cmd);
}

t_command *parse_cmd(char *input, char **envp)
{
	t_command	*cmd;
	int			i;

	i = 0;
	cmd = calloc(sizeof(t_command) + MAXLINE * sizeof(char *), 1);
	cmd->redirect[1] = -1;
	cmd->redirect[0] = -1;
	cmd->argv = ft_split(input, " \t\n\r");
	cmd->name = cmd->argv[0];
	while(cmd->argv[i])
		i++;
	cmd->argv[i] = NULL;
	cmd->path = ft_find_cmd(cmd);
	cmd->builtin = parseBuiltin(cmd);
	cmd->argc = ft_word_count(input, ' ');
	cmd->envp = envp;
	return (cmd);
}

t_pipeline	*parse_pipeline(char *input, char **envp)
{
	t_pipeline	*pipeline;
	char		*copy;
	char		*tmp;
	int			num_cmds;
	int			i;

	// count pipe characters in input
	i = 0;
	num_cmds = 0;
	copy = ft_strdup(input);
	tmp = copy;
	while (*copy)
	{
		if (*copy == '|')
			num_cmds++;
		copy++;
	}
	copy = tmp;
	num_cmds++;
	pipeline = calloc(sizeof(t_pipeline) + num_cmds * sizeof(t_command*), 1);
	pipeline->num_cmds = num_cmds;
	tmp = ft_strsep(&copy, "|");
	while (tmp)
	{
		pipeline->cmds[i++] = parse_cmd(tmp, envp);
		tmp = ft_strsep(&copy, "|");
	}
	return (pipeline);
}

// Helper function to print a command struct

void print_command(t_command* command) {
	char** arg = command->argv;
	int i = 0;

	printf("progname: %s\n", command->name);
	printf("redirect[0]: %d\n", command->redirect[0]);
	printf("redirect[1]: %d\n", command->redirect[1]);
	while (arg[i] != NULL) 
	{
		printf("argv[%d]: %s\n", i, arg[i]);
		++i;
	}
}

void print_pipeline(t_pipeline* pipeline) 
{
	t_command** cmd = pipeline->cmds;
	int i = 0;

	printf("num_cmds: %d\n", pipeline->num_cmds);

	for (i = 0; i < pipeline->num_cmds; ++i) 
	{
		printf("cmds[%d]:\n", i);
		print_command(cmd[i]);
	}
}

void eval(char *input, char **envp)
{
	t_pipeline	*pipeline;
	int			num_pipes;
	int			(*pipes)[2];
	int			i;

	pipeline = parse_pipeline(input, envp);
	num_pipes = pipeline->num_cmds - 1;
	pipes = calloc(sizeof(int[2]), num_pipes);
	i = 1;
	while (i < pipeline->num_cmds)
	{
		pipe(pipes[i - 1]);
		pipeline->cmds[i]->redirect[STDIN_FILENO] = pipes[i - 1][0];				// read end of previous pipe
		pipeline->cmds[i - 1]->redirect[STDOUT_FILENO] = pipes[i - 1][1]; 			// write end of current pipe
		i++;
	}
	//print_pipeline(pipeline);
	i = 0;
	if (pipeline->num_cmds == 1)
		run(pipeline->cmds[0]);
	else
		while (i < pipeline->num_cmds)
			run_with_fork(pipeline->cmds[i++], num_pipes, pipes);
	close_pipes(num_pipes, pipes);
	i = 0;
	while (i < pipeline->num_cmds)
	{
		wait(NULL);
		i++;
	}
	//ft_free_pipeline(pipeline);
	free(pipeline);
}

int	main(int argc, char **argv, char **envp) 						// don't forget --char **envp-- argument
{
	char	*input;										// buffer for readline
	char	*cwd;
	char	*tmp;

	setting_signal();
	if (argv[0] != NULL)
	{
		while (argc > 0)
		{
			tmp = getcwd(NULL, 0);
			cwd = ft_relative_path(tmp);
			printf("%ld%ld ", (long)getpid(), (long)getppid());
			printf("%s➜%s %s%s%s ", BLUE, RESET, GREEN, cwd, RESET);
			free(cwd);
			input = readline(YELLOW "~" RESET " ");
			if (!input)
			{
				printf("\n");
				break;
			}
			else if (ft_strcmp(input, "\n"))
				add_history(input);
			else if (feof(stdin))
				exit (0);
			if (*input)
				eval(input, envp);						// Evaluate input
			free(tmp);
		}
	}
	return (0);
}
