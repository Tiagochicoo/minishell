/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 12:08:18 by tiago             #+#    #+#             */
/*   Updated: 2022/11/21 17:49:01 by tpereira         ###   ########.fr       */
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

typedef struct s_token
{
	int		tok_type;
	char	*token;
}		t_token;

typedef struct s_command
{
	int			argc;				// number of args
	char		**argv;				// arguments list
	t_list		*args;				// linked list 
	int			tok_type;
	char		**envp;				// environment variables
	void		*input;
	void		*output;
	// operator type enum
	t_builtin	cmd_type;			// builtin type enum
	struct s_command		*next;
	struct s_command		*prev;
}			t_command;

//type of each token
//has a binary representation
//can be used with a bit mask
typedef enum e_tok_type
{
	ERROR = 1,
	TEXT = 2,
	//quotes
	SINGLE_Q = 4,
	DOUBLE_Q = 8,
	//quotes bit mask
	QUOTE = 12,
	//Operators
	INPUT = 16,
	OUTPUT = 32,
	HFILE = 64,
	APPEND = 128,
	PIPE = 256,
	OR = 512,
	AND = 1024,
	//bitmask for operators INP, OUT, APP, PIPE, HFILE, OR, AND
	OPERATOR = 2032,
	OPEN_P = 2048,
	CLOSE_P = 4096,
	//bitmask for OPEN_P, CLOSE_P
	PARENTHESES = 6144,
	SEMICOLON = 8192,
}			t_tok_type;

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
void	run_sys_cmd(t_command *cmd, char *cmd_argv0);

void	ft_ft(void);

//functions in linked_list.c file
void		dll_add_back(t_command **lst, t_command *new);
void		dll_add_front(t_command **lst, t_command *new);
void		dll_add_after(t_command *current, t_command *new);
void		dll_put_before(t_command **lst, t_command *current, t_command *move);
void		dll_content_del(t_command *current);
void		dll_delone(t_command **lst, t_command *current);
void		dll_clear(t_command **lst);
t_command	*dll_last(t_command *lst);
t_command	*dll_penultimate(t_command *lst);
int			dll_size(t_command *lst);
t_command	*dll_new(t_list *lst);
void		dll_gethead(t_command **lst);
void		dll_iter(t_command *lst, void (*f)(t_command *current, t_list *head, t_list *lst));

//functions in parser.c file
char		*ft_strpbrk(const char *s, const char *accept);
t_list		*ft_lstbefore(t_list *lst, t_list *current);
int			getnextc(char const *s, char c);
int			count_up_to_chr(char *string, char *pos);
void		get_text(t_list **lst, char **input, char *tmp);
t_list		**get_quote(t_list **lst, char **input, char *tmp, char quote);
void		quote_parser(t_list **lst, char *input);
void		del_tok(void *a);
void		ft_lst_iter(t_list *lst);
int			strarrsize(char **arr);
int			ft_strsize(char *str);
bool		is_lastchar(char *str, char cmp);
t_token		*new_token(char *content, t_tok_type type);
void		col_split_parser(t_list **lst, t_list *current, char **split, t_command **dll_list);
void		col_end_parser(t_list **lst, t_list *current, t_command **dll_list);
void 		ft_print_split(char **str);
void		column_parser_cmds(t_list **lst, t_list *i, t_command **dll);
t_command	*column_parser(t_list **lst);
char		*find_operator(char *str);
int			what_operator(char *op);
int			size_of_op(int op);
t_list		*add_par_node(int op);
void		split_on_op(t_list *current, char *pos, int op_size, int op);
void		operator_parser(t_command *list);
char		*trim_whitespace(char *str);
void		trim_whitespace_parser(t_command *list);
void		split_and_or(t_command *current);
char		**split_on_spaces(t_token *tok);
void		free_char_array(char **array);
char		**add_to_array(char **array, char **new);
void 		put_node_token_on_argv(t_command *list);
int			is_quotes(t_token *token);
int			is_operator(t_token *token);
void 		split_on_parenthesis(t_command *current);
void 		split_on_operators(t_command *list);
void 		remove_empty_nodes(t_command *list);
void 		print_shit(t_command *list);
void		print_argv(t_command *list);

#endif
