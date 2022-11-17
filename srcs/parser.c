/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:04:10 by tpereira          #+#    #+#             */
/*   Updated: 2022/11/17 14:16:44 by mimarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

/**
 * @brief LibC function to find a character from a list of chars
 * 
 * @param s string to go through
 * @param accept list of chars to find in 's'
 * @return char* 
 */
char	*ft_strpbrk(const char *s, const char *accept)
{
	int	i;
	int	j;

	i = 0;
	while (s[i])
	{
		j = 0;
		while (accept[j])
		{
			if (s[i] == accept[j])
				return ((char *)s + i);
			j++;
		}
		i++;
	}
	return (NULL);
}

//Gets the previous node from the current one
t_list	*ft_lstbefore(t_list *lst, t_list *current)
{
	t_list	*previous;

	previous = NULL;
	if (lst == NULL || current == NULL)
		return (NULL);
	if (lst == current)
		return (NULL);
	while (lst != current)
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

void	get_text(t_list **lst, char **input, char *tmp)
{
	t_token	*tok;

	if (count_up_to_chr(*input, tmp) == 0)
		return ;
	//make a linked list node with text up to quote
	tok = malloc(sizeof(t_token));
 	tok->tok_type = TEXT;
	tok->token = ft_substr(*input, 0, count_up_to_chr(*input, tmp)); //get text up to tmp
	ft_lstadd_back(lst, ft_lstnew(tok));
}

t_list	**get_quote(t_list **lst, char **input, char *tmp, char quote)
{
	char	*tmpb;
	t_token	*tok;

	if (quote == '\"')
		tmpb = ft_strchr((tmp + 1), '\"');
	else
		tmpb = ft_strchr((tmp + 1), '\'');
	if (tmpb == NULL)
		perror("Error: unclosed quote");
	//make a linked list node with text up to quote
	get_text(lst, input, tmp);
	//add a linked list node with quote after text
	if (count_up_to_chr(tmp, tmpb) == 0)
		return (NULL);
	tok = malloc(sizeof(t_token));
	if (quote == '\"')
		tok->tok_type = DOUBLE_Q;
	else
		tok->tok_type = SINGLE_Q;
	tok->token = ft_substr(tmp + 1, 0, count_up_to_chr(tmp, tmpb - 1)); //get text between quotes
	ft_lstadd_back(lst, ft_lstnew(tok));
	*input = tmpb + 1;
	return (lst);
}

void	quote_parser(t_list **lst, char *input)
{
	char	*inpt;
	char	*tmp;
	
	inpt = input;
	while (inpt)
	{
		tmp = ft_strpbrk(inpt, "\"'");			//try to find quotes
		if (tmp == NULL)					//get last piece of text
		{
			tmp = inpt;
			while (*tmp)
				tmp++;
			get_text(lst, &inpt, tmp);
			break ;
		}
		else if (ft_strchr(tmp, '\''))			//has single quote
			get_quote(lst, &inpt, tmp, '\'');
		else if (ft_strchr(tmp, '"'))			//has double quote
			get_quote(lst, &inpt, tmp, '"');
	}
}

//command arg1 "arg2" arg3 < input > output; command2 arg1 arg2 arg3 

//TEXT			 Double_Q Text
//command arg1 -> arg2 -> arg3 < input > output; command2 arg1 arg2 arg3 

//				  TEXT			 Double_Q Text
//t_command(args: command arg1 -> arg2 -> arg3 < input > output )=>t_command(args: command2 arg1 arg2 arg3)

//t_command->args->t_list->content->t_token->t_tok_type tipo
//										   ->t_token    string


void	del_tok(void *a)
{
	t_token	*content;

	content = (t_token *)a;
	free(content->token);
	free(content);
	content = NULL;
}

void	ft_lst_iter(t_list *lst)
{
	static int	iter = 0;

	printf("\n\n");
	while (lst)
	{
		printf("token: \"%s\" -> ", ((t_token *)lst->content)->token);
		lst = lst->next;
	}
	iter++;
	printf("\n\n");
}

int	strarrsize(char **arr)
{
	int		i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

int ft_strsize(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

bool	is_lastchar(char *str, char cmp)
{
	char	*tmp;
	int		size;


	tmp = ft_strchr(str, (int)cmp);
	size = ft_strsize(str);
	if (!tmp)
		return (false);
	if (*tmp == str[size - 1]) // off by one?
		return (true);
	return (false);
}

t_token	*new_token(char *content, t_tok_type type)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	tok->token = ft_strdup(content);
	tok->tok_type = type;
	return (tok);
}

void	col_split_parser(t_list **lst, t_list *current, char **split, t_command **dll_list)
{
	t_list		*next;
	t_list		*before;
	int			size;
	int			i;
	
	size = strarrsize(split);
	before = ft_lstbefore(*lst, current);
	if (before)
		before->next = NULL; //make sure prev node doesnt point to free'd memory
	else
		*lst = NULL; // Or that lst doen't either (if it's the first node)
	next = current->next; //get next node
	ft_lstdelone(current, del_tok); //delete current node
	//split the linked list into 2; lst and next
	ft_lstadd_back(lst, ft_lstnew(new_token(split[0], TEXT))); //add first token at the end of the lst list
	dll_add_back(dll_list, dll_new(*lst)); //add lst list to first t_command
	//what if there is a garbage value after the NUL
	if (size > 2)
	{
		i = 1;
		while(split[i + 1])
		{
			dll_add_back(dll_list, dll_new(ft_lstnew(new_token(split[i], TEXT))));
			i++;
		}
	}
	//make the last bit a sll node and assign it to lst
	ft_lstadd_front(&next, ft_lstnew(new_token(split[size - 1], TEXT))); //add last token at the begining of next list. off by one?
	*lst = next;
}

//does this work? Não percam o proximo episódio porque nós tambem não
//ps: não, não funcionava.
void	col_end_parser(t_list **lst, t_list *current, t_command **dll_list)
{
	char	*dup;
	char	*content;
	t_list	*tmp;
	t_list	*before;

	content = ((t_token *)current->content)->token;
	dup = ft_strndup(content, (ft_strsize(content) - 1));
	free(content);
	((t_token *)current->content)->token = dup;
	tmp = current->next; //get next node
	before = ft_lstbefore(*lst, current);
	if (before)
		before->next = NULL; //get previous and set it to null
	else
		current->next = NULL;
	dll_add_back(dll_list, dll_new(*lst)); //make new dll node and add it to the back of the dll
	*lst = tmp;//lst = next node 
}

void 	ft_print_split(char **str)
{
	int i;

	i = 0;
	while (str[i])
	{
		printf("%s\n", str[i]);
		i++;
	}
}

//because fuck norminette
void	column_parser_cmds(t_list **lst, t_list *i, t_command **dll)
{
	char		**split;
	
	split = ft_split(((t_token *)i->content)->token, ';'); // split
	col_split_parser(lst, i, split, dll);
	ft_delete_split_arr(split);
}

//check what it does with just quotes
t_command	*column_parser(t_list **lst)
{
	t_list		*i;
	t_command	*dll;
	t_token		*tmp;

	dll = NULL;
	i = *lst;
	while (i)
	{
		tmp = ((t_token *)i->content);
		printf("%d : %s\n", tmp->tok_type, tmp->token);
		if (tmp->tok_type == TEXT && ft_strpbrk(tmp->token, ";")
			&& !is_lastchar(tmp->token, ';'))
		{
			column_parser_cmds(lst, i, &dll);
			i = *lst;
			continue ;
		}
		else if (tmp->tok_type == TEXT && is_lastchar(tmp->token, ';'))
		{
			col_end_parser(lst, i, &dll);
			i = *lst;
			continue ;
		}
		i = i->next;
	}
	//add either the whole list if skipped while or the last part to a t_command
	if (*lst)
		dll_add_back(&dll, dll_new(*lst));
	return (dll);
}

char	*find_operator(char *str)
{
	return (ft_strpbrk(str, "><|&()"));
}

int	what_operator(char *op)
{
	//check if there is more than one char

	if (ft_strsize(op) > 1)
	{
		if (*op == '>' && *(op + 1) == '>')
			return (APPEND);
		else if (*op == '<' && *(op + 1) == '<')
			return (HFILE);
		else if (*op == '|' && *(op + 1) == '|')
			return (OR);
		else if (*op == '&' && *(op + 1) == '&')
			return (AND);
	}
	if (*op == '>')
	 	return (OUTPUT);
	else if (*op == '<')
	 	return (INPUT);
	else if (*op == '|')
	 	return (PIPE);
	else if (*op == '(')
	 	return (OPEN_P);
	else if (*op == ')')
		return (CLOSE_P);
	return (ERROR);
}

int	size_of_op(int op)
{
	if (op == APPEND || op == HFILE || op == OR || op == AND)
		return (2);
	else if (op == PIPE || op == OUTPUT || op == INPUT || op == OPEN_P || op == CLOSE_P)
	 	return (1);
	else
		return (0);
}

t_list *add_par_node(int op)
{
	t_token	*new;
	t_list	*new_node;
	char	*str;

	if (op == OPEN_P)
		str = ft_strdup("(");
	else
		str = ft_strdup(")");
	new = new_token(str, op);
	new_node = ft_lstnew(new);
	return (new_node);
}

//current is the current node with content t_token
//pos is the position of the operator in the t_token->token string
//op_size is the size of the operator 1 or 2 chars
//op is the operator type
//if there is a token before the operator, add it to the current node
//add a node with just the operator
//if there is a token after the operator, add it to the next node
void	split_on_op(t_list *current, char *pos, int op_size, int op)
{
	t_token	*new;
	t_list	*new_node;
	char	*str;
	t_token	*tok;
	char	*prev;
	t_list	*tmp;

	tok = (t_token *)current->content;
	prev = tok->token;
	if (pos != tok->token) //if there is a token before the operator (aka token not in begining)
	{
		str = ft_strndup(tok->token, (pos - prev));
		tok->token = str;
		//add new node with the operator
		new = new_token(ft_strndup(pos, op_size), op);
		new_node = ft_lstnew(new);
		tmp = current->next;
		current->next = new_node;
		new_node->next = tmp;
		current = new_node;
	}
	else //else put the operator in the current node
	{
		str = ft_strndup(pos, op_size);
		tok->token = str;
		tok->tok_type = op;
	}
	if (*(pos + op_size)) //if there is a token after the operator
	{
		new = new_token(ft_strdup(pos + op_size), TEXT);
		new_node = ft_lstnew(new);
		tmp = current->next;
		current->next = new_node;
		new_node->next = tmp;
	}
	free(prev);
}

void	operator_parser(t_command *list)
{
	t_token	*content;
	t_list	*node;
	char	*op;
	int		t_op;

	while (list)
	{
		//t_command(dll_node)->t_list(ll_node)->t_token(content)
		node = ((t_list *)list->args);
		while (node)
		{
			content = ((t_token *)node->content);
			op = find_operator(content->token);
			if (op != NULL)
			{
				t_op = what_operator(op);
				//find if node only contains operator
				if (ft_strlen(content->token) == (size_t)size_of_op(t_op))
				{
					content->tok_type = t_op;
					node = node->next;
					continue ;
				}
				if (content->tok_type != DOUBLE_Q && content->tok_type != SINGLE_Q) //Don't split if in quotes
					split_on_op(node, op, size_of_op(t_op), t_op);
			}
			node = node->next;
		}
		list = list->next;
	}
}

//needs to free char *str
//trim whitespace from the beginning and end of a string
char	*trim_whitespace(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*new;

	i = 0;
	j = 0;
	len = ft_strsize(str);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[len - 1] == ' ' || str[len - 1] == '\t')
		len--;
	new = malloc(sizeof(char) * (len - i + 1));
	while (i < len)
		new[j++] = str[i++];
	new[j] = '\0';
	return (new);
}

//for each node in the t_list if the token is not a OPEN_P or CLOSE_P or DOUBLE_Q or SINGLE_Q
//doon't forguet to free the t_token content after triming whitespace
void	trim_whitespace_parser(t_command *list)
{
	t_token	*content;
	t_list	*node;
	char	*temp;

	while (list)
	{
		node = ((t_list *)list->args);
		while (node)
		{
			content = ((t_token *)node->content);
			temp = content->token;
			if (content->tok_type != OPEN_P && content->tok_type != CLOSE_P
				&& content->tok_type != DOUBLE_Q && content->tok_type != SINGLE_Q)
			{
				content->token = trim_whitespace(temp);
				free(temp);
			}
			node = node->next;
		}
		list = list->next;
	}
}

//what about parenthesis?
//if the t_list node token is AND or OR, make a new t_command node with the current and next t_list nodes
void split_and_or(t_command *current)
{
	t_list		*node;
	t_list		*temp;

	node = current->args;
	while (node)
	{
		if (((t_token *)node->content)->tok_type == AND || ((t_token *)node->content)->tok_type == OR)
		{
			dll_add_after(current, dll_new(node));
			temp = ft_lstbefore(current->args, node);
			if (!temp)
				perror("no command before AND/OR");
			temp->next = NULL;
		}
		else
			node = node->next;
	}
}

void print_shit(t_command *list)
{
	t_token	*content;
	t_list	*node;
	int		n;
	int		op;
	char	*text;

	n = 1;
	while (list)
	{
		printf("\n");
		printf("%d => ", n);
		node = ((t_list *)list->args);
		while (node)
		{
			op = ((t_token *)node->content)->tok_type;
			content = ((t_token *)node->content);
			//turn tok_type to text
			if (op == ERROR)
				text = "ERROR";
			else if (op == APPEND)
				text = "APPEND";
			else if (op == OUTPUT)
			 	text = "OUTPUT";
			else if (op == HFILE)
				text = "HFILE";
			else if (op == INPUT)
			 	text = "INPUT";
			else if (op == OR)
				text = "OR";
			else if (op == PIPE)
			 	text = "PIPE";
			else if (op == AND)
				text = "AND";
			else if (op == OPEN_P)
			 	text = "(";
			else if (op == CLOSE_P)
				text = ")";
			else if (op == DOUBLE_Q)
			 	text = "DOUBLE_Q";
			else if (op == SINGLE_Q)
				text = "SINGLE_Q";
			else
				text = "TEXT";
			printf("%s: \"%s\" -> ", text, content->token);
			node = node->next;
		}
		printf("\n");
		n++;
		list = list->next;
	}
}