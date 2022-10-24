/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:39:13 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/24 22:49:18 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	echo(t_command *cmd) 
{
	int i = 1;
	while (cmd->argv[i] != NULL && ft_strcmp(cmd->argv[i], "-n") == 0)
		i++;
	while (cmd->argv[i]) 
	{
		printf("%s ", cmd->argv[i]);
		i++;
	}
	if (cmd->argv[1] == NULL || ft_strcmp(cmd->argv[1], "-n") != 0)
		printf("\n");
	if (cmd->argv[1] != NULL && ft_strcmp(cmd->argv[1], "-n") == 0)
		printf("\b");
}

void	cd(t_command *cmd) 
{
	if (cmd->argc == 1) {
		chdir(getenv("HOME"));
	} else if (cmd->argc == 2) {
		if (chdir(cmd->argv[1]) == -1) {
			printf("cd: %s: No such file or directory\n", cmd->argv[1]);
		}
	} else {
		printf("cd: too many arguments\n");
	}
}

void	pwd(void) 
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s \n", cwd);
}

void	export(t_command *cmd) 
{
	int i = 1;
	while (cmd->argv[i]) {
		if (ft_strchr(cmd->argv[i], '=')) {
			char *key = ft_substr(cmd->argv[i], 0, ft_strchr(cmd->argv[i], '=') - cmd->argv[i]);
			char *value = ft_substr(cmd->argv[i], ft_strchr(cmd->argv[i], '=') - cmd->argv[i] + 1, ft_strlen(cmd->argv[i]));
			setenv(key, value, 1);
			free(key);
			free(value);
		} else {
			printf("export: `%s': not a valid identifier\n", cmd->argv[i]);
		}
		i++;
	}
}

void	unset(t_command *cmd) 
{
	int i = 1;
	while (cmd->argv[i]) {
		unsetenv(cmd->argv[i]);
		i++;
	}
}

void	env(char **envp) 
{
	int i = 0;
	while (envp[i]) {
		printf("%s \n", envp[i]);
		i++;
	}
}

void	ft_ft(void) // prints ASCII art of 42 Lisboa
{
	printf("/\\ \\\\ \\    /'___`\\    /\\ \\       __         /\\ \\                         \n");
	printf("\\ \\ \\\\ \\  /\\_\\ /\\ \\   \\ \\ \\     /\\_\\    ____\\ \\ \\____    ___      __     \n");
	printf(" \\ \\ \\\\ \\_\\/_/// /__   \\ \\ \\  __\\/\\ \\  /',__\\\\ \\ '__`\\  / __`\\  /'__`\\   \n");
	printf("  \\ \\__ ,__\\ // /_\\ \\   \\ \\ \\L\\ \\\\ \\ \\/\\__, `\\\\ \\ \\L\\ \\/\\ \\L\\ \\/\\ \\L\\.\\_ \n");
	printf("   \\/_/\\_\\_//\\______/    \\ \\____/ \\ \\_\\/\\____/ \\ \\_,__/\\ \\____/\\ \\__/.\\_\\ \n");
	printf("      \\/_/  \\/_____/      \\/___/   \\/_/\\/___/   \\/___/  \\/___/  \\/__/\\/_/\n");
}
