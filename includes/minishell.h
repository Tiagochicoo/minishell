/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:08:18 by tiago             #+#    #+#             */
/*   Updated: 2023/01/04 17:00:33 by tpereira         ###   ########.fr       */
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
	char		*cmd;							// command name
	int			redirect[2];					// redirect input/output
	int			background;						// run in background?
	int			argc;							// number of args
	char		**argv;							// arguments list
	char		**envp;							// environment variables
	t_builtin	builtin;						// is argv[0] a builtin command?
	struct s_command	*next;					// next command in pipeline
	struct s_command	*head;					// head of the pipeline
}			t_command;	

typedef struct s_cmd
{
	char	**envp;								// environment variables
	char	*cmd;								// command name
	int		redirect[2];						// redirect input/output
	char	*args[];							// arguments: must be NULL terminated
}		t_cmd;

typedef struct s_pipeline
{
	int			num_cmds;						// number of commands in pipeline
	char		**envp;							// environment variables
	t_cmd		*cmds[];						// array of commands
}			t_pipeline;

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
void	ft_exit(t_command *cmd);

// UTILS.C
char	*ft_relative_path(char *cwd);
char	*ft_find_cmd(t_command *cmd);
void	file_exists(t_command *cmd, int bg);

// EXECUTOR
void	run(t_command *cmd);
void	run_sys_cmd(t_command *cmd, char *cmd_argv0, int bg);

void	ft_ft(void);

// PIPES
t_pipeline	*parse_pipeline(char *input);
pid_t	run_redir(t_cmd *cmd, int num_pipes, int (*pipes)[2]);

// DESTROY

void    ft_free_cmd(t_command *cmd);


#endif
