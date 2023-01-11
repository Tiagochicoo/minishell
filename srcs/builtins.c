/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:39:13 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/11 16:18:16 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	echo(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->argv[i] != NULL && ft_strcmp(cmd->argv[i], "-n") == 0)		// -n flag is optional and can be used multiple times
		i++;
	while (cmd->argv[i] && cmd->argv[i][0] == '$') 							// if the argument starts with $, print the value of the environment variable
	{
		if (getenv(&cmd->argv[i][1]))
			printf("%s ", getenv(&cmd->argv[i][1]));
		i++;
	}
	while (cmd->argv[i])
		printf("%s ", cmd->argv[i++]);
	if (cmd->argv[1] == NULL || ft_strcmp(cmd->argv[1], "-n") != 0)
		printf("\n");
	if (cmd->argv[1] != NULL && ft_strcmp(cmd->argv[1], "-n") == 0)
		printf("\b");
	//ft_free_cmd(cmd);
}

void	cd(t_command *cmd)
{
	if (cmd->argc == 1)
		chdir(getenv("HOME"));
	else if (cmd->argc == 2)
	{
		if (ft_strcmp(cmd->argv[1], "-") == 0)
			chdir(getenv("OLDPWD"));
		if (chdir(cmd->argv[1]) == -1)
			printf("cd: %s: No such file or directory\n", cmd->argv[1]);
	}
	else
		printf("cd: too many arguments\n");
	//ft_free_cmd(cmd);
}

void	pwd(void)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (cwd == NULL)
		perror("pwd command error!");
	printf("%s \n", cwd);
	free(cwd);
}

void	export(t_command *cmd)
{
	int		i;
	char	*key;
	char	*value;
	char	*argv;

	i = 1;
	while (cmd->argv[i])
	{
		argv = cmd->argv[i];
		if (ft_strchr(argv, '='))
		{
			key = ft_substr(argv, 0, ft_strchr(argv, '=') - argv);
			value = ft_substr(argv, ft_strchr(argv, '=') - argv
					+ 1, ft_strlen(argv));
			setenv(key, value, 1);
			free(key);
			free(value);
		}
		else
			printf("export: `%s': not a valid identifier\n", argv);
		i++;
	}
	//ft_free_cmd(cmd);
}

void	unset(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->argv[i])
		unsetenv(cmd->argv[i++]);
	//ft_free_cmd(cmd);
}

void	env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		printf("%s \n", envp[i++]);
}

void	ft_exit(t_command *cmd)
{
	int num;

	num = 2;
	if (cmd->argc == 1)
		exit(0);				// need to implement exit status from last command executed
	else if (cmd->argc == 2)
	{
		if (ft_isdigit(cmd->argv[1][0]) == 0)
			printf("exit: %s: numeric argument required\n", cmd->argv[1]);
		else
			num = ft_atoi(cmd->argv[1]);
		exit(num);
	}
	else
	{
		printf("exit: too many arguments\n");
		exit(1);
	}
	num = ft_atoi(cmd->argv[1]);
	ft_free_cmd(cmd);
	exit(0);
}

/*
	
	prints ASCII art of 42 Lisboa

*/
void	ft_ft(void)
{
	printf("%s __ __      ____       %s__                    __%s\n", YELLOW, GREEN, RESET);
	printf("%s/\\ \\\\ \\    /'___`\\%s    /\\ \\       __         /\\ \\                         %s\n", YELLOW, GREEN, RESET);
	printf("%s\\ \\ \\\\ \\  /\\_\\ /\\ \\%s   \\ \\ \\     /\\_\\    ____\\ \\ \\____    ___      __     %s\n", YELLOW, GREEN, RESET);
	printf("%s \\ \\ \\\\ \\_\\/_/// /__%s   \\ \\ \\  __\\/\\ \\  /',__\\\\ \\ '__`\\  / __`\\  /'__`\\   %s\n", YELLOW, GREEN, RESET);
	printf("%s  \\ \\__ ,__\\ // /_\\ \\%s   \\ \\ \\L\\ \\\\ \\ \\/\\__, `\\\\ \\ \\L\\ \\/\\ \\L\\ \\/\\ \\L\\.\\_ %s\n", YELLOW, GREEN, RESET);
	printf("%s   \\/_/\\_\\_//\\______/ %s   \\ \\____/ \\ \\_\\/\\____/ \\ \\_,__/\\ \\____/\\ \\__/.\\_\\ %s\n", YELLOW, GREEN, RESET);
	printf("%s      \\/_/  \\/_____/ %s     \\/___/   \\/_/\\/___/   \\/___/  \\/___/  \\/__/\\/_/%s\n", YELLOW, GREEN, RESET);
}
