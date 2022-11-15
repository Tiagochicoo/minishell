/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:04:10 by tpereira          #+#    #+#             */
/*   Updated: 2022/11/15 17:41:15 by mimarque         ###   ########.fr       */
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
	if (!*(op + 1))
		return(ERROR);
	if (*op == '>' && *(op + 1) == '>')
		return (APPEND);
	else if (*op == '>')
	 	return (OUTPUT);
	else if (*op == '<' && *(op + 1) == '<')
		return (HFILE);
	else if (*op == '<')
	 	return (INPUT);
	else if (*op == '|' && *(op + 1) == '|')
		return (OR);
	else if (*op == '|')
	 	return (PIPE);
	else if (*op == '&' && *(op + 1) == '&')
		return (AND);
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

void	split_on_op(t_list *lst, char *pos, int op_size, int op)
{
	char	*temp; //node content
	char	*prev; //text before operator
	char	*next; //text after the operator
	t_list	*new; //new node to be added (text after operator)
	t_list	*par; //node if open or close parenthesis
	t_list	*seg; //next node
	int		size; //size of the text before or after the operator

	
	temp = ((t_token *)lst->content)->token;
	prev = NULL;
	next = NULL;
	seg = lst->next;
	//split token
	
	size = pos - temp; //get size up to operator
	if (size > 0)
		prev = ft_strndup(temp, size);
	size = ft_strsize(temp) - (size + op_size); //get size after the operator
	if (size > 0)
		next = ft_strndup((pos + op_size), size); //copy after the operator
	//replace content on current node
	//prev stays on the same node
	//if there are parenthesis, add them after prev
	//next goes to a new node
	if (prev && next)
	{
		((t_token *)lst->content)->token = prev;
		//add new node
		new = ft_lstnew(new_token(next, op));
		if (op == OPEN_P || op == CLOSE_P)
		{
			par = add_par_node(op);
			((t_token *)new->content)->tok_type = TEXT;
			lst->next = par;
			par->next = new;
			new->next = seg;
		}
		else
		{
			new->next = seg;
			lst->next = new;
		}
	}
	else if (prev)
	{
		((t_token *)lst->content)->token = prev;
		if (op == OPEN_P || op == CLOSE_P)
		{
			par = add_par_node(op);
			lst->next = par;
			par->next = seg;
		}
		else
			lst->next = seg;
	}
	else if (next)
	{
		//((t_token *)lst->content)->tok_type = op;
		if (op == OPEN_P || op == CLOSE_P)
		{	
			if(op == OPEN_P)
			{
				((t_token *)lst->content)->tok_type = OPEN_P;
				((t_token *)lst->content)->token = ft_strdup("(");
			}
			else
			{
				((t_token *)lst->content)->tok_type = CLOSE_P;
				((t_token *)lst->content)->token = ft_strdup(")");
			}
			lst->next = ft_lstnew(new_token(next, op));
			lst->next->next = seg;
		}
		else
		{
			((t_token *)lst->content)->token = next;
			lst->next = seg;
		}
	}
	if (((bool)prev ^ (bool)next) && !(op == OPEN_P || op == CLOSE_P)) //if one is null and the other is not
		((t_token *)lst->content)->tok_type = op;
	free(temp);
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