/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 16:01:56 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/24 22:31:58 by tpereira         ###   ########.fr       */
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
	cmd->argv[cmd->argc] = NULL;							// argument list must end with a NULL pointer
	if (cmd->argc == 0)										// ignore blank line
		return 1;
	cmd->builtin = parseBuiltin(cmd);
	if ((is_bg = (*cmd->argv[cmd->argc-1] == '&')) != 0)	// should job run in background?
		cmd->argv[--cmd->argc] = NULL;
	return is_bg;
}

char	*ft_find_cmd(t_command *cmd)					// needs to free return value!!!!
{
	char	*path;
	char	*cmd_path;
	char	*temp_path;
	char	**all_paths;
	int		i;

	i = 0;
	path = getenv("PATH");
	all_paths = ft_split(path, ':');
	while (all_paths[i])
	{
		temp_path = ft_strjoin(all_paths[i], "/");
		cmd_path = ft_strjoin(temp_path, cmd->argv[0]);
		if (!access(cmd_path, X_OK))
		{
			free(temp_path);
			break ;
		}
		free(cmd_path);
		free(temp_path);
		i++;
	}
	return (cmd_path);
}

void run_sys_cmd(t_command *cmd, int bg)
{
	pid_t childPid;
	char	*path;

	if ((childPid = fork()) < 0)					// fork a child process	
		error("fork() error");
	else if (childPid == 0)							// I'm the child and could run a command
	{
		path = ft_find_cmd(cmd);
		if (execve(path, cmd->argv, cmd->envp) < 0)	// EXECVE != EXECVP
		{
			printf("%sCommand not found: %s%s\n", RED, RESET, cmd->argv[0]);
			exit(0);
		}
		printf("executed\n");
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

void eval(char *input, char **envp) 
{
	int			background;					// should job run in background?
	t_command	cmd;						// parsed command

	background = parse(input, &cmd);	 	// parse line into command struct
	cmd.envp = envp;						// set envp
	if (background == -1)					// parse error
		return;
	if (cmd.argv[0] == NULL)				// empty line - ignore
		return;
	if (cmd.builtin == NONE)
		run_sys_cmd(&cmd, background);
	else
		run_builtin_cmd(&cmd);
}

int	main(int argc, char **argv, char **envp) 
{
	char	*input;							// buffer for readline
	char	*cwd;

	signal(SIGINT, handler);
	if (argv[0] != NULL)
	{
		while (argc)
		{
			cwd = ft_relative_path(getcwd(NULL, 0));
			printf("%s➜%s %s%s%s ", BLUE, RESET, GREEN, cwd, RESET);
			input = readline(YELLOW "~" RESET " ");
			if (!input)
			{
				printf("\n");
				break;
			}
			else if (ft_strcmp(input, "\n") > 0)
				add_history(input);
			eval(input, envp);				// Evaluate input
		}
	}
}
