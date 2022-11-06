/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mimarque <mimarque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 12:32:04 by mimarque          #+#    #+#             */
/*   Updated: 2022/11/06 00:23:39 by mimarque         ###   ########.fr       */
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
	tmp = dll_last(lst);
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
	if (lst == current)
		*lst = current->next;
	if (current->next != NULL)
        current->next->prev = current->prev;
	if (current->prev != NULL)
        current->prev->next = current->next;
	dll_content_del(current);
	free(current);
}

//Acho que funciona mas não tenho a certeza
void	dll_clear(t_command **lst)
{
	t_command *tmp;

	if (!lst)
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

void	dll_iter(t_command *lst, int property, void (*f)(void *))
{
	if (!lst || !property || !f)
		return ;
	while (lst)
	{
		if(property == 1)
			f(lst->argv);
		if(property == 2)
			f(lst->args);
		if(property == 3)
			f(lst->envp);
		lst = lst->next;
	}
}

//falta dll_new mas acho melhor fazer contigo por causa dos argumentos