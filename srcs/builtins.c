/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 16:39:13 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/24 17:42:39 by tpereira         ###   ########.fr       */
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
		printf("[%d]\n", i);
		printf("%s ", cmd->argv[i]);
		i++;
	}
	if (ft_strcmp(cmd->argv[1], "-n") != 0)
		printf("\n");
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
