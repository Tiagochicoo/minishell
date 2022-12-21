/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 16:01:56 by tpereira          #+#    #+#             */
/*   Updated: 2022/12/21 11:50:35 by tpereira         ###   ########.fr       */
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

int parse(const char *input, t_command *cmd) 
{
	static char	array[MAXLINE];					// local copy of command line
	const char	delims[10] = " \t\r\n\"\'";		// argument delimiters
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
		line += ft_strspn(line, delims);		// skip delimiters
		if (line >= endline)
			break;
		token = line + ft_strcspn(line, delims);// Find token delimiter
		*token = '\0';							// terminate the token
		cmd->argv[cmd->argc++] = line;			// Record token as the token argument
		if (cmd->argc >= MAXARGS - 1)				// Check if argv is full
			break;
		line = token + 1;
	}
	cmd->argv[cmd->argc] = NULL;				// argument list must end with a NULL pointer
	if (cmd->argc == 0)							// ignore blank line
		return 1;
	cmd->builtin = parseBuiltin(cmd);
	is_bg = (*cmd->argv[cmd->argc-1] == '&');
	if (is_bg)									// should job run in background?
		cmd->argv[--cmd->argc] = NULL;
	return is_bg;
}

void	file_exists(t_command *cmd, int bg)
{
	char	*tmp;

	tmp = ft_find_cmd(cmd);
	if (tmp != NULL)
		run_sys_cmd(cmd, tmp, bg);
	else if (!access(cmd->argv[0], F_OK))
	{
		printf("File Exists\n");
		if (!access(cmd->argv[0], X_OK))
		{
			printf("Has access\n");
			run_sys_cmd(cmd, cmd->argv[0], bg);
		}
		else
			perror("Error");
	}
}

void	run_sys_cmd(t_command *cmd, char *cmd_argv0, int bg)
{
	pid_t childPid;
	char	*path;

	path = cmd_argv0;
	if ((childPid = fork()) < 0)					// fork a child process	
		error("fork() error");
	else if (childPid == 0)							// I'm the child and could run a command
	{
		if (execve(path, cmd->argv, cmd->envp) < 0)	// EXECVE != EXECVP
		{
			printf("%sError: command not found: %s%s\n", RED, RESET, cmd->argv[0]);
			exit(0);
		}
		free(path);
	}
	else	// I'm the parent. Shell continues here.
	{
		 if (bg)
			printf("Child in background [%d]\n",childPid);
		 else
			wait(&childPid);
	}
}

void run_builtin_cmd(t_command *cmd) 
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
		exit(0);
	else if (cmd->builtin == FT)
		ft_ft();
}

void ft_add_cmd(t_command *head, t_command *cmd)
{
	t_command tmp;

	tmp = *head;
	printf("1test\n");
	if (tmp.argv == NULL)
		head = cmd;
	else
	{
		while (tmp.next != NULL)
			tmp = *tmp.next;
		tmp.next = cmd;
	}
	printf("2test\n");
	cmd->next = NULL;
}

t_command *ft_str2cmd(char *str, t_command *cmd_list)
{
	t_command *cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		error("Memory allocation error!");
	cmd->argc = ft_word_count(str, ' ');
	cmd->argv = ft_split(str, ' ');
	cmd->builtin = parseBuiltin(cmd);
	cmd->next = NULL;
	cmd->envp = cmd_list->envp;
	if (cmd_list->argv == NULL)
		cmd_list = cmd;
	else
	{
		cmd->head = cmd_list;
		while (cmd_list->next != NULL)
			cmd_list = cmd_list->next;
		cmd_list->next = cmd;
	}
	return (cmd_list);
}

void	execute(t_command *cmd_list, int background)
{
	while (cmd_list->next != NULL)
	{
		if (cmd_list->builtin)
			run_builtin_cmd(cmd_list);
		else
			file_exists(cmd_list, background);
		cmd_list = cmd_list->next;
	}
	if (cmd_list->builtin)
		run_builtin_cmd(cmd_list);
	else
		file_exists(cmd_list, background);
}

void eval(char *input, char **envp) 
{
	int			background;									// should job run in background?
	t_command	*cmd_list;									// parsed commands linked list
	char		**cmds;										// array of commands coming from input
	int			i;

	i = 0;
	if (!input)
		return ;
	cmds = ft_split_many(input, "|<>()"); 					// split input into commands
	cmd_list = (t_command *)malloc(sizeof(t_command));
	while (cmds[i])
		i++;
	i = 0;
	while (cmds[i])
	{
		cmd_list->envp = envp;
		cmd_list = ft_str2cmd(cmds[i], cmd_list); 				// convert string to command
		// ft_add_cmd(cmd_list.head, &cmd_list);				// add command to the cmd_list
		background = parse(cmds[i], cmd_list);					// parse command line into cmd_list struct
		printf("token -> %s\n", cmd_list->argv[i]);
		i++;
	}
	execute(cmd_list, background);
}

int	main(int argc, char **argv, char **envp) 
{
	char	*input;										// buffer for readline
	char	*cwd;

	setting_signal();
	if (argv[0] != NULL)
	{
		while (argc > 0)
		{
			cwd = ft_relative_path(getcwd(NULL, 0));
			printf("%s➜%s %s%s%s ", BLUE, RESET, GREEN, cwd, RESET);
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
		}
	}
	return (0);
}
