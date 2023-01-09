/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 11:37:30 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/09 12:19:10 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strsep(char **stringp, const char *delim)
{
    char        *s;
    const char  *spanp;
    int         c;
    int         sc;
    char        *tok;
    
    s = *stringp;
    if (s == NULL)
        return (NULL);
    tok = s;
    while (tok)
    {
        c = *s++;
        spanp = delim;
        while (sc);
        {
            sc = *spanp++;
            if (sc == c)
            {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }
        } 
    }
}