/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:08:18 by tiago             #+#    #+#             */
/*   Updated: 2022/10/23 17:12:15 by tpereira         ###   ########.fr       */
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

# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define BLUE "\x1B[34m"
# define PURPLE "\x1B[35m"
# define RESET "\x1b[0m"

# define MAXLINE 1024
// maxargs used in struct, can't be variable so we have to use define
# define MAXARGS 128

struct command {
	int argc;					// number of args
	char *argv[MAXARGS];		// arguments list
	enum builtin_t 				// is argv[0] a builtin command?
	{
		NONE,
		EXIT,
		JOBS,
		BG,
		FG
	} builtin;
};

typedef struct s_data
{
	char	*pwd;
	char	*rel_pwd;
	char	*buffer;
	char	**cmds;
	int		cmd_count;
	char	*env;
	
}	t_data;

typedef struct s_command
{
	char	*str;
	char	**env;
	char	**argv;
	int		fd_in;
	int		fd_out;
	char	*file_name;
}	t_command;

void	handler(int signum);



#endif
