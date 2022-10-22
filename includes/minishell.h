/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:08:18 by tiago             #+#    #+#             */
/*   Updated: 2022/10/22 13:27:04 by mimarque         ###   ########.fr       */
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

/*
	pwd			current working directory (needs to be freed)
	rel_pwd		current folder (pointer to last chars of "pwd")
	buffer		input (needs to be freed)
	cmds		list of commands (needs to free split)
	cmd_count	number of commands
	env			evironment pointer
*/
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
void	setting_signal();

#endif
