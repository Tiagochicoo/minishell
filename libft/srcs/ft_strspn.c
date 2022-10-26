/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 21:13:20 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/26 21:15:09 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** The strspn() function computes the length/position of the maximum initial
segment of the string pointed to by s1 which consists entirely of characters 
in the string pointed to by s2.
*/

int ft_strspn(const char *s, const char *accept)
{
    int i;
    int j;

    i = 0;
    while (s[i])
    {
        j = 0;
        while (accept[j])
        {
            if (s[i] == accept[j])
                break ;
            j++;
        }
        if (!accept[j])
            return (i);
        i++;
    }
    return (i);
}
