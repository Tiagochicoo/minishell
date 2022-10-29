/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 16:01:56 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/29 14:32:42 by mimarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_builtin parseBuiltin(t_command *cmd) 
{
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

int parse(const char *input, t_command *cmd) 
{
	static char	array[MAXLINE];					// local copy of command line
	const char	delims[10] = " \t\r\n";			// argument delimiters
	char		*line;							// ptr that traverses command line
	char		*token;							// ptr to the end of the current arg
	char		*endline;						// ptr to the end of the input string
	int			is_bg;							// background job?

	line = array;
	if (input == NULL)
		error("command line is NULL\n");
	(void) ft_strncpy(line, input, MAXLINE);
	endline = line + ft_strlen(line);
	cmd->argc = 0;								// build argv list
	while (line < endline)
	{
		line += ft_strspn(line, delims);		// skip delimiters
		if (line >= endline)
			break;
		token = line + ft_strcspn(line, delims);// Find token delimiter
		*token = '\0';							// terminate the token
		cmd->argv[cmd->argc++] = line;			// Record token as the token argument
		if (cmd->argc >= MAXARGS-1)				// Check if argv is full
			break;
		line = token + 1;
	}
	cmd->argv[cmd->argc] = NULL;				// argument list must end with a NULL pointer
	if (cmd->argc == 0)							// ignore blank line
		return 1;
	cmd->builtin = parseBuiltin(cmd);
	is_bg = (*cmd->argv[cmd->argc-1] == '&');
	if (is_bg)									// should job run in background?
		cmd->argv[--cmd->argc] = NULL;
	return is_bg;
}

void	file_exists(t_command *cmd, int bg)
{
	char	*tmp;

	tmp = ft_find_cmd(cmd);
	if (tmp != NULL)
		run_sys_cmd(cmd, tmp, bg);
	else if (!access(cmd->argv[0], F_OK))
	{
		printf("File Exists\n");
		if (!access(cmd->argv[0], X_OK))
		{
			printf("Has access\n");
			run_sys_cmd(cmd, cmd->argv[0], bg);
		}
		else
			perror("Error");
	}
}

void	run_sys_cmd(t_command *cmd, char *cmd_argv0, int bg)
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
		 if (bg)
			printf("Child in background [%d]\n",childPid);
		 else
			wait(&childPid);
	}
}

void run_builtin_cmd(t_command *cmd) 
{
	if (cmd->builtin == ECHO)
		echo(cmd);
	else if (cmd->builtin == CD)
		cd(cmd);
	else if (cmd->builtin == PWD)
		pwd();
	else if (cmd->builtin == EXPORT)
		export(cmd);
	else if (cmd->builtin == ENV)
		env(cmd->envp);
	else if (cmd->builtin == UNSET)
		unset(cmd);
	else if (cmd->builtin == EXIT)
		exit(0);
	else if (cmd->builtin == FT)
		ft_ft();
}

void eval(char *input, char **envp) 
{
	int			background;					// should job run in background?
	t_command	cmd;						// parsed command

	background = parse(input, &cmd);	 	// parse line into command struct
	cmd.envp = envp;						// set envp
	if (background == -1)					// parse error
		return;
	if (cmd.argv[0] == NULL)				// empty line - ignore
		return;
	if (cmd.builtin == NONE)
		file_exists(&cmd, background);
	else
		run_builtin_cmd(&cmd);
}

/**
 * @brief LibC function to find a character from a list of chars
 * 
 * @param s string to go through
 * @param accept list of chars to find in 's'
 * @return char* 
 */
char	*ft_strpbrk(const char *s, char *accept)
{
	char	*a;

	while (*s != '\0')
	{
		a = accept;
		while (*a != '\0')
			if (*a++ == *s)
				return ((char *) s);
		++s;
	}
	return (NULL);
}

//Gets the previous node from the current one
t_list	*ft_lstbefore(t_list *lst, t_list *current)
{
	t_list	*previous;

	previous = NULL;
	if (!lst || !current)
		return (NULL);
	while (lst->next != current)
	{
		previous = lst;
		lst = lst->next;
	}
	return (previous);
}

int	getnextc(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	return (i);
}

int	count_up_to_chr(char *string, char *pos)
{
	return (pos - string);
}

typedef enum e_tok_type
{
	TEXT,
	SINGLE_Q,
	DOUBLE_Q,
	COMMAND
}		t_tok_type;

typedef struct s_token
{
	int		tok_type;
	char	*token;
}		t_token;

void	get_text(t_list **lst, char *input, char *tmp)
{
	t_token	*tok;

	if (count_up_to_chr(input, tmp) == 0)
		return ;
	//make a linked list node with text up to quote
	tok = malloc(sizeof(t_token));
	tok->tok_type = TEXT;
	tok->token = ft_substr(input, 0, count_up_to_chr(input, tmp)); //get text up to tmp
	ft_lstadd_back(lst, ft_lstnew(tok));
}

void	get_quote(t_list **lst, char *input, char *tmp, char quote)
{
	char	*tmpb;
	t_token	*tok;

	if (quote == '\"')
		tmpb = ft_strpbrk((tmp+1),"\"");
	else
		tmpb = ft_strpbrk((tmp+1),"'");
	if (tmpb == NULL)
		perror("Error: unclosed quote");
	//make a linked list node with text up to quote
	get_text(lst, input, tmp);
	//add a linked list node with quote after text
	if (count_up_to_chr(tmp, tmpb) == 0)
		return ;
	tok = malloc(sizeof(t_token));
	if (quote == '\"')
		tok->tok_type = DOUBLE_Q;
	else
		tok->tok_type = SINGLE_Q;
	tok->token = ft_substr(tmp + 1, 0, count_up_to_chr(tmp, tmpb)); //get text between quotes
	ft_lstadd_back(lst, ft_lstnew(tok));
	return (lst);
}

void	quote_parser(t_list **lst, char *input)
{
	char	*inpt;
	char	*tmp;
	char	*tmpb;

	inpt = input;
	while (inpt != '\0')
	{
		tmp = ft_strpbrk(inpt,"\"'"); //try to find quotes
		if (tmp == '\'') //has single quote
			get_quote(lst, inpt, tmp, '\'');
		else if (tmp == '"') //has double quote
			get_quote(lst, inpt, tmp, '"');
		else if (tmp == NULL) //get last piece of text
		{
			tmp = inpt;
			while (tmp != '\0')
				tmp++;
			get_text(lst, inpt, tmp);
			break ;
		}
		inpt = tmpb;
	}
}

// /!\ EDGE CASE ; at the end without anything after untested
t_list	*column_splitter(t_list *inpt)
{
	t_list	*list;
	t_token	*tok;
	char	**split;
	int		j;

	list = NULL;
	split = ft_split(((t_token *)inpt->content)->token, ';');
	while (split[j])
	{
		tok = malloc(sizeof(t_token));
		tok->token = ft_strdup(split[j]);
		tok->tok_type = COMMAND;
		ft_lstadd_back(&list, ft_lstnew(tok));
		j++;
	}
	if (split != NULL)
		ft_delete_split_arr(split);
	return (list);
}

void	del_tok(void *a)
{
	t_token	*content;

	content = (t_token *)a;
	free(content->token);
	free(content);
	content = NULL
}

void	column_parser(t_list **lst)
{
	t_list	*i;
	t_list	*tmp;
	t_list	*prev;
	t_list	*next;

	i = *lst;
	while (i)
	{
		if (((t_token *)i->content)->tok_type == TEXT)
		{
			tmp = column_splitter(i);
			prev = ft_lstbefore(*lst, i);
			//edge case if there
			//if there is no previous and we split current node
			if (prev = NULL && tmp != NULL)
			{
				next = (*lst)->next; //save next if there's any
				ft_lstdelone(*lst, del_tok); //delete current node since it is going to be replaced
				*lst = tmp; //replace node
				ft_lstlast(*lst)->next = next; //add trailing nodes
			}
			else if (tmp != NULL)
			{
				next = i->next; //save next if there's any
				ft_lstdelone(i, del_tok); //delete current node since it is going to be replaced
				prev = tmp; //replace node
				ft_lstlast(*lst)->next = next; //add trailing nodes
			}
		}
		i = i->next;
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
