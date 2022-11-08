/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:08:18 by tiago             #+#    #+#             */
/*   Updated: 2022/11/08 13:08:25 by mimarque         ###   ########.fr       */
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
#include <stdbool.h>

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

// 

typedef struct s_command 
{
	int			argc;				// number of args
	char		*argv[MAXARGS];		// arguments list
	t_token		*args;				// linked list 
	char		**envp;				// environment variables
	void		*input;				
	void		*output;
	// operator type enum
	struct s_command		*next;
	struct s_command		*prev;
}			t_command;

typedef enum e_tok_type
{
	TEXT,
	SINGLE_Q,
	DOUBLE_Q,
	COMMAND
}		t_tok_type;

typedef enum e_operators
{
	INPT,
	OTPT,
	HFIL,
	APND,
	PIPE,
	OR,
	AND
}		t_operators;

typedef struct s_token
{
	int		tok_type;
	char	*token;
}		t_token;

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

// PARSER
char *ft_strpbrk(const char *s, const char *accept);
t_list *ft_lstbefore(t_list *lst, t_list *current);
int getnextc(char const *s, char c);
int count_up_to_chr(char *string, char *pos);
void get_text(t_list **lst, char **input, char *tmp);
void get_quotes(t_list **lst, char **input, char *tmp);
void quote_parser(t_list **lst, char *input);
t_list *column_splitter(t_list *inpt);
void del_tok(void *a);
void ft_lst_iter(t_list *lst);
t_command *column_parser(t_list **lst);

//linked list
void	dll_add_back(t_command **lst, t_command *new);
void	dll_add_front(t_command **lst, t_command *new);
void	dll_add_after(t_command *current, t_command *new);
void	dll_content_del(t_command *current);
void	dll_delone(t_command **lst, t_command *current);
void	dll_clear(t_command **lst);
t_command	*dll_last(t_command *lst);
t_command	*dll_penultimate(t_command *lst);
int	dll_size(t_command *lst);
void	dll_iter(t_command *lst, int property, void (*f)(void *));

#endif
