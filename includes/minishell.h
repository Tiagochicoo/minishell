/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:08:18 by tiago             #+#    #+#             */
/*   Updated: 2022/10/26 21:48:19 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/includes/libft.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define BLUE "\x1B[34m"
# define PURPLE "\x1B[35m"
# define RESET "\x1b[0m"

# define MAXLINE 1024
// maxargs used in struct, can't be variable so we have to use define
# define MAXARGS 128

typedef enum s_builtin
{
	NONE,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	FT,
}			t_builtin;

typedef struct s_command 
{
	int			argc;				// number of args
	char		*argv[MAXARGS];		// arguments list
	char		**envp;				// environment variables
	t_builtin	builtin;			// is argv[0] a builtin command?
}			t_command;

// typedef struct s_token
// {
// 	char		*token;
// 	int			type;
// 	t_token		*next;
// 	t_token		*prev;
// }			t_token;

// SIGNALS
void	handler(int signum);
void	setting_signal();

// BUILTINS
void	env(char **envp);
void	unset(t_command *cmd);
void	export(t_command *cmd);
void	pwd(void);
void	cd(t_command *cmd);
void	echo(t_command *cmd);

// UTILS.C
char	*ft_relative_path(char *cwd);
char	*ft_find_cmd(t_command *cmd);

// EXECUTOR
void	run_sys_cmd(t_command *cmd, char *cmd_argv0, int bg);

void	ft_ft(void);

#endif
