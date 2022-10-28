/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 19:34:35 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/28 18:45:19 by tpereira         ###   ########.fr       */
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
	int		i;
	char	*c;

	i = 0;
	while (str[i])
	{
		if (ft_isalnum(str[i]))
			i++;
		else
			return (0);
	}
	return (1);
}
