/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 19:34:35 by tpereira          #+#    #+#             */
/*   Updated: 2021/02/20 15:55:49 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}

int	ft_str_isalnum(char *str)
{
	int	i;
	int	c;

	i = 0;
	while (*str)
	{
		c = ft_atoi(str);
		if (ft_isalpha(c) || ft_isdigit(c))
			str++;
		else
			return (0);
	}
	return (1);
}
