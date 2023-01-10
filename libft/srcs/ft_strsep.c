/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 11:37:30 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/10 11:17:55 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strsep(char **stringp, const char *delim)
{
    int         c;
    int         sc;
    char        *s;
    char        *tok;
    const char  *spanp;
    
    s = ft_strdup(*stringp);
    if (s == NULL)
        return (NULL);
    tok = s;
    while (tok)
    {
        c = *s++;
        spanp = delim;
        sc = *spanp;
        while (sc)
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
    return (NULL);
}