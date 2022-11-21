/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 16:01:56 by tpereira          #+#    #+#             */
/*   Updated: 2022/11/21 20:01:23 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_builtin parseBuiltin(t_command *cmd) 
{
	//int i;
	
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

t_command	*parser(char *input)
{
	t_list		*lst;
	t_command	*list;

	lst = NULL;
	if (input == NULL)
		error("command line is NULL\n");
	quote_parser(&lst, input);
	list = column_parser(&lst);
	operator_parser(list);
	trim_whitespace_parser(list);
	remove_empty_nodes(list);
	print_shit(list);
	split_on_operators(list);
	print_shit(list);
	
	return(list);
}

void	file_exists(t_command *cmd)
{
	char	*tmp;

	tmp = ft_find_cmd(cmd);
	if (tmp != NULL)
		run_sys_cmd(cmd, tmp);
	else if (!access(cmd->argv[0], F_OK))
	{
		if (!access(cmd->argv[0], X_OK))
		{
			run_sys_cmd(cmd, cmd->argv[0]);
		}
		else
			perror("Error");
	}
}

void	run_sys_cmd(t_command *cmd, char *cmd_argv0)
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
		wait(&childPid);
	}
}

//change this to be the first argv
void run_builtin_cmd(t_command *cmd) 
{
	if (cmd->cmd_type == ECHO)
		echo(cmd);
	else if (cmd->cmd_type == CD)
		cd(cmd);
	else if (cmd->cmd_type == PWD)
		pwd();
	else if (cmd->cmd_type == EXPORT)
		export(cmd);
	else if (cmd->cmd_type == ENV)
		env(cmd->envp);
	else if (cmd->cmd_type == UNSET)
		unset(cmd);
	else if (cmd->cmd_type == EXIT)
		exit(0);
	else if (cmd->cmd_type == FT)
		ft_ft();
}

void eval(char *input, char **envp) 
{
	t_command	*cmds;						// parsed command

	cmds = parser(input);
	while (cmds->next)
	{
		cmds->cmd_type = parseBuiltin(cmds);
		cmds->envp = envp;						// set envp
		if (cmds->argv == NULL)				// empty line - ignore
			return ;
		if (cmds->cmd_type == NONE)
			file_exists(cmds);
		else
			run_builtin_cmd(cmds);
		cmds = cmds->next;
	}	
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
