/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:04:10 by tpereira          #+#    #+#             */
/*   Updated: 2022/11/11 21:44:33 by mimarque         ###   ########.fr       */
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
		printf("token: %s -> ", ((t_token *)lst->content)->token);
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
void	col_end_parser(t_list **lst, t_list *current, t_command **dll_list)
{
	char	*dup;
	char	*content;
	t_list	*tmp;

	content = ((t_token *)current->content)->token;
	dup = ft_strndup(content, (ft_strsize(content) - 1));
	free(content);
	((t_token *)current->content)->token = dup;
	tmp = current->next; //get next node
	ft_lstbefore(*lst, current)->next = NULL; //get previous and set it to null
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
	dll_add_back(&dll, dll_new(*lst));
	return (dll);
}

char	*find_operator(char *str)
{
	return (ft_strpbrk(str, "><|&"));
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
	return (ERROR);
}

int	size_of_op(int op)
{
	if (op == APPEND || op == HFILE || op == OR || op == AND)
		return (2);
	else if (op == PIPE || op == OUTPUT || op == INPUT)
	 	return (1);
	else
		return (0);
}

void	split_on_op(t_list *lst, char *pos, int op_size, int op)
{
	char	*temp;
	char	*prev;
	char	*next;
	t_list	*new;
	int		size;
	
	temp = ((t_token *)lst->content)->token;
	//split token
	size = (pos - 1) - temp; //get size up to operator
	prev = malloc(sizeof(char) * size); 
	ft_strncpy(prev, temp, size);
	size = ft_strsize(temp) - (size + op_size); //get size after the operator
	next = malloc(sizeof(char) * size);
	ft_strncpy(next, (pos - 1 + op_size), size); //copy after the operator
	//replace content on current node
	free(temp);
	((t_token *)lst->content)->token = prev;
	//add new node
	new = ft_lstnew(new_token(next, op));
	new->next = lst->next;
	lst->next = new;
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
				split_on_op(node, op, size_of_op(t_op), t_op);
			}
			node = node->next;
		}
		list = list->next;
	}
}

void print_shit(t_command *list)
{
	t_token	*content;
	t_list	*node;
	int		n;

	n = 1;
	while (list)
	{
		printf("\n");
		printf("%d => ", n);
		node = ((t_list *)list->args);
		while (node)
		{
			content = ((t_token *)node->content);
			printf("%d: %s -> ", content->tok_type, content->token);
			node = node->next;
		}
		printf("\n");
		n++;
		list = list->next;
	}
}