/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 12:32:04 by mimarque          #+#    #+#             */
/*   Updated: 2022/11/20 00:46:43 by mimarque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	dll_add_back(t_command **lst, t_command *new)
{
	t_command *tmp;

	if (!new)
		return ;
	if (*lst == NULL) 
	{
        new->prev = NULL;
        *lst = new;
        return ;
    }
	tmp = dll_last(*lst);
	tmp->next = new;
	new->prev = tmp;
}

void	dll_add_front(t_command **lst, t_command *new)
{
	if (!new)
		return ;
	new->next = *lst;
	new->prev = NULL;
	if ((*lst) != NULL)
        (*lst)->prev = new;
	(*lst) = new;
}

void	dll_add_after(t_command *current, t_command *new)
{
	if (!current || !new)
		return ;
	new->next = current->next;
	current->next = new;
	new->prev = current;
	if (new->next != NULL)
		new->next->prev = new;
}

//'move' goes before 'current'
void	dll_put_before(t_command **lst, t_command *current, t_command *move)
{
	t_command *prevn;
	t_command *nextn;

	if (!lst || !current || !move || (current == move))
		return ;
	prevn = move->prev;
	nextn = move->next;
	//cut move
	if (*lst == move) //if move is first elem in the list
		*lst = move->next;
	if (prevn) //if move is not in the beginning
		prevn->next = nextn;
	if (nextn) //if move is not in the end
		nextn->prev = prevn;
	//insert move before current
	if (*lst == current) //if current is first elem in the list
		*lst = move; //make head point to move
	move->prev = current->prev; //make move point to the same current is pointing
	if (current->prev) //if current is not the first elem in the list
		current->prev->next = move; //make the elem before current point to move
	current->prev = move; //make current point to move
	move->next = current; //make move point to current
}

void	dll_content_del(t_command *current)
{
	ft_delete_split_arr(current->argv);
	//ft_lstclear(args, del);
	//free(envp);?
	//free(input);?
	//free(output);?
}

//head and current node
void	dll_delone(t_command **lst, t_command *current)
{
	if (!lst || !current)
		return ;
	if (*lst == current)
		*lst = current->next;
	if (current->next != NULL)
        current->next->prev = current->prev;
	if (current->prev != NULL)
        current->prev->next = current->next;
	dll_content_del(current);
	free(current);
}

//Acho que funciona mas n??o tenho a certeza
void	dll_clear(t_command **lst)
{
	if (!*lst)
		return ;
	while (*lst)
		dll_delone(lst, *lst);
}

t_command	*dll_last(t_command *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

t_command	*dll_penultimate(t_command *lst)
{
	if (lst == NULL)
		return (NULL);
	return ((dll_last(lst)->prev));
}

int	dll_size(t_command *lst)
{
	int			count;

	count = 0;
	while (lst)
	{
		lst = lst->next;
		count++;
	}
	return (count);
}

t_command	*dll_new(t_list *lst)
{
	t_command *new;

	new = malloc(sizeof(t_command));
	new->args = lst;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	dll_gethead(t_command **lst)
{
	if (!lst)
		return ;
	if ((*lst)->prev == NULL)
		return ;
	else
		while (*lst != NULL)
			*lst = (*lst)->prev;
}

//for each t_list in each t_command do function f
//pass current t_command, current t_list and head of t_list
void	dll_iter(t_command *lst, void (*f)(t_command *current, t_list *head, t_list *lst))
{
	t_list *current;

	if (!lst || !f)
		return ;
	while (lst)
	{
		current = lst->args;
		while (current)
		{
			f(lst, lst->args, current);
			current = lst->args->next;
		}
		lst = lst->next;
	}
}

