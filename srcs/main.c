/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 16:01:56 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/10 17:20:32 by tpereira         ###   ########.fr       */
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

int	file_exists(t_command *cmd)
{
	char	*tmp;

	printf("files_exists()\n");
	tmp = ft_find_cmd(cmd);
	if (tmp != NULL)
		run_sys_cmd(cmd, tmp);
	else if (!access(cmd->argv[0], F_OK))
	{
		if (!access(cmd->argv[0], X_OK))
			run_sys_cmd(cmd, cmd->argv[0]);
		else
			perror("Error");
	}
	free(tmp);
	return (0);
}

void	run_sys_cmd(t_command *cmd, char *cmd_argv0)
{
	pid_t	child_pid;
	char	*path;

	path = cmd_argv0;
	child_pid = fork();											// Fork a child process											
	if (child_pid < 0)
		error("fork() error");
	else if (child_pid == 0)										// I'm the child and could run a command
	{
		if (execve(path, cmd->argv, cmd->envp) < 0)				// EXECVE != EXECVP
		{
			printf("%sError: command not found: %s%s\n", RED, RESET, cmd->argv[0]);
			exit(0);
		}
		free(path);
	}
	else														// I'm the parent. Shell continues here.
		wait(&child_pid);									// &child_pid == NULL
}

// void run_builtin_cmd(t_command *cmd) 
// {
// 	if (cmd->builtin == ECHO)
// 		echo(cmd);
// 	else if (cmd->builtin == CD)
// 		cd(cmd);
// 	else if (cmd->builtin == PWD)
// 		pwd();
// 	else if (cmd->builtin == EXPORT)
// 		export(cmd);
// 	else if (cmd->builtin == ENV)
// 		env(cmd->envp);
// 	else if (cmd->builtin == UNSET)
// 		unset(cmd);
// 	else if (cmd->builtin == EXIT)
// 		ft_exit(cmd);
// 	else if (cmd->builtin == FT)
// 		ft_ft();
// }

// void ft_add_cmd(t_command *head, t_command *cmd)
// {
// 	t_command tmp;

// 	tmp = *head;
// 	if (tmp.argv == NULL)
// 		head = cmd;
// 	else
// 	{
// 		while (tmp.next != NULL)
// 			tmp = *tmp.next;
// 		tmp.next = cmd;
// 	}
// 	cmd->next = NULL;
// }

// t_command *ft_str2cmd(char *str, t_command *cmd_list)
// {
// 	t_command	*cmd;

// 	if (!cmd_list->argc)
// 	{
// 		cmd = cmd_list;
// 		cmd->argc = ft_word_count(str, ' ');
// 		cmd->argv = ft_split(str, " ");
// 		cmd->background = parse(str, cmd);
// 		if (cmd->argc)
// 			cmd->builtin = parseBuiltin(cmd);
// 		cmd->next = NULL;
// 	}
// 	else
// 	{
// 		cmd = (t_command *)malloc(sizeof(t_command));
// 		if (!cmd)
// 			error("Memory allocation error!");
// 		cmd->envp = cmd_list->envp;
// 		cmd->head = cmd_list->head;
// 		cmd->argc = ft_word_count(str, ' ');
// 		cmd->argv = ft_split(str, " ");
// 		cmd->background = parse(str, cmd);
// 		if (cmd->argc)
// 			cmd->builtin = parseBuiltin(cmd);
// 		cmd->next = NULL;		
// 		cmd_list->next = cmd;
// 	}
// 	return (cmd);
// }

void	run(t_command *cmd, int num_pipes, int (*pipes)[2])
{
	// if (cmd->builtin)
	// 	run_builtin_cmd(cmd);
	// else
	// {
		printf("run()\n");
		execute_redir(cmd, num_pipes, pipes);
		ft_free_cmd(cmd);
	// }
}

// void	execute(t_command *cmd_list)
// {
// 	while (cmd_list->next != NULL)
// 	{
// 		run(cmd_list);
// 		cmd_list = cmd_list->next;
// 	}
// 	run(cmd_list);
// }

// void	evaluate(char *input, char **envp)
// {
// 	t_pipeline	*pipeline;										// parsed commands linked list
// 	int			num_pipes;										// number of pipes in input
// 	int			(*pipes)[2];									// pipe file descriptors
// 	int			i;

// 	pipeline = parse_pipeline(input);
// 	num_pipes = pipeline->num_cmds - 1;
// 	pipes = calloc(sizeof(int[2]), num_pipes);
// 	i = 1;
// 	while (i < pipeline->num_cmds)
// 	{
// 		pipe(pipes[i - 1]);
// 		pipeline->cmds[i]->redirect[STDIN_FILENO] = pipes[i - 1][0]; 		// read end of previous pipe
// 		pipeline->cmds[i]->redirect[STDOUT_FILENO] = pipes[i][1]; 			// write end of current pipe
// 		i++;
// 	}
// 	i = 0;
// 	while (i < pipeline->num_cmds)
// 	{
// 		run_redir(pipeline->cmds[i], num_pipes, pipes);
// 		i++;
// 	}
// }

t_command *parse_cmd(char *input, char **envp)
{
	t_command	*cmd;

	cmd = calloc(sizeof(t_command) + MAXLINE * sizeof(char *), 1);
	cmd->redirect[1] = -1;
	cmd->redirect[0] = -1;
	cmd->argv = ft_split(input, " \t\n\r");
	cmd->name = cmd->argv[0];
	cmd->path = ft_find_cmd(cmd);
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
		printf("num_pipes = %d\n", num_pipes);
		printf("pipe()\n");
		printf("pipes[0][0] = %d\n", pipes[0][0]);
		printf("pipes[0][1] = %d\n", pipes[0][1]);
		printf("pipe %d: [%d] [%d]\n", i, pipes[i - 1][0], pipes[i - 1][1]);
		pipeline->cmds[i]->redirect[STDIN_FILENO] = pipes[i - 1][0];				// read end of previous pipe
		pipeline->cmds[i - 1]->redirect[STDOUT_FILENO] = pipes[i - 1][1]; 			// write end of current pipe
		printf("pipeline->cmds[i]->redirect[STDIN_FILENO] = %d\n", pipeline->cmds[i]->redirect[STDIN_FILENO]);
		printf("pipeline->cmds[i - 1]->redirect[STDOUT_FILENO] = %d\n", pipeline->cmds[i - 1]->redirect[STDOUT_FILENO]);
		i++;
	}
	i = 0;
	while (i < pipeline->num_cmds)
	{
		run(pipeline->cmds[i], num_pipes, pipes);
		i++;
	}
	close_pipes(num_pipes, pipes);
	i = 0;
	while (i < pipeline->num_cmds)
	{
		wait(NULL);
		i++;
	}
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
