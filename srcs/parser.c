/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 17:04:10 by tpereira          #+#    #+#             */
/*   Updated: 2022/11/08 13:23:34 by mimarque         ###   ########.fr       */
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
	while (inpt != (void *)0)
	{
		tmp = ft_strpbrk(inpt, "\"'");			//try to find quotes
		if (tmp == NULL)
			break;
		if (ft_strchr(tmp, '\''))				//has single quote
			get_quote(lst, &inpt, tmp, '\'');
		else if (ft_strchr(tmp, '"'))			//has double quote
			get_quote(lst, &inpt, tmp, '"');
		else if (tmp == NULL)					//get last piece of text
		{
			tmp = inpt;
			while (tmp != (void *)0)
				tmp++;
			get_text(lst, &inpt, tmp);
			break ;
		}
	}
}

//command arg1 "arg2" arg3 < input > output; command2 arg1 arg2 arg3 
//command arg1 -> "arg2" -> arg3 < input > output; command2 arg1 arg2 arg3 
//t_command(command arg1 -> "arg2" -> arg3 < input > output )=>t_command(command2 arg1 arg2 arg3)
//t_command(command arg1 -> "arg2" -> arg3 )=>t_command(< input)=> t_command(> output )=>t_command(command2 arg1 arg2 arg3)
//t_command(< input)=>t_command(command arg1 -> "arg2" -> arg3 )=> t_command(> output )=>t_command(command2 arg1 arg2 arg3)

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
		printf("token: %s\n", ((t_token *)lst->content)->token);
		lst = lst->next;
	}
	iter++;
	printf("iter: %d\n", iter);
}

int	strarrsize(char **arr)
{
	int		i;

	while (arr)
	{
		arr++;
		i++;
	}
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
	if (tmp == str[size - 1]) // off by one?
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
	int			size;
	int			i;
	
	size = strarrsize(split);
	ft_lstbefore(*lst, current)->next = NULL; //make sure prev node doesnt point to free'd memory
	next = current->next; //get next node
	ft_lstdelone(current, del_tok); //delete current node
	//split the linked list into 2; lst and next
	ft_lstadd_back(lst, ft_lstnew(new_token(split[0], TEXT))); //add first token at the end of the lst list
	ft_lstadd_front(&next, ft_lstnew(new_token(split[size - 1], TEXT))); //add last token at the begining of next list. off by one?
	dll_add_back(dll_list, dll_new(*lst)); //add lst list to first t_command
	//what if there is a garbage value after the NUL
	if (size > 2)
	{
		i = 1;
		while(split[i + 1] != '\0')
		{
			dll_add_back(dll_list, dll_new(ft_lstnew(new_token(split[i], TEXT))));
			i++;
		}
	}
	//make the last bit a sll node and assign it to lst
	ft_lstadd_front(&next, ft_lstnew(new_token(split[size], TEXT)));
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

//check what it does with just quotes
t_command	*column_parser(t_list **lst)
{
	t_list		*i;
	char		**split;
	t_command	*dll;

	i = *lst;
	while (i)
	{
		if (((t_token *)i->content)->tok_type == TEXT
			&& ft_strpbrk(((t_token *)i->content)->token, ";")
			&& !is_lastchar(((t_token *)i->content)->token, ';'))
		{
			split = ft_split(((t_token *)i->content)->token, ';'); // split
			col_split_parser(lst, i, split, &dll);
			ft_delete_split_arr(split);
		}
		else if (((t_token *)i->content)->tok_type == TEXT
				&& is_lastchar(((t_token *)i->content)->token, ';'))
			col_end_parser(lst, i, &dll);
		i = i->next;
	}
	//add either the whole list if skipped while or the last part to a t_command
	dll_add_back(dll, dll_new(*lst));
	return (dll);
}

//GNU C STRSEP
size_t	strcspn(char *str1, const char *str2)
{
	char	*p;
	char	*spanp;
	char	c;
	char	sc;

	/*
	 * Stop as soon as we find any character from s2.  Note that there
	 * must be a NUL in s2; it suffices to stop when we find that, too.
	 */
	p = str1;
	while (true)
	{
		c = *p++;
		spanp = str2;
		do {
			if ((sc = *spanp++) == c)
				return (p - 1 - str1);
		} while (sc != 0);
	}
}

char	*ft_strsep(char **stringp, const char *delim)
{
	char	*begin;
	char	*end;

	begin = *stringp;
	if (begin == NULL)
		return NULL;
	/* Find the end of the token.  */
	end = begin + ft_strcspn(begin, delim);
	if (*end)
	{
		/* Terminate the token and set *STRINGP past NUL character.  */
		*end++ = '\0';
		*stringp = end;
	}
	else
		/* No more delimiters; this is the last token.  */
		*stringp = NULL;
	return (begin);
}

char	*find_operator(char *str)
{
	return (ft_strpbrk(str, "><|&"));
}

//what do if op+1 is NUL?
int	what_operator(char *op)
{
	if (*op == '>' && *(op + 1) == '>')
		return (APND);
	else if (*op == '>')
	 	return (OTPT);
	if (*op == '<' && *(op + 1) == '<')
		return (HFIL);
	else if (*op == '<')
	 	return (INPT);
	if (*op == '|' && *(op + 1) == '|')
		return (OR);
	else if (*op == '|')
	 	return (PIPE);
	if (*op == '&' && *(op + 1) == '&')
		return (AND);
}