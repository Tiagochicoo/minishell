/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:12:21 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/24 22:21:51 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char    *ft_relative_path(char *cwd)
{
    char    **tmp;
    int     i;

    i = 0;
    tmp = ft_split(cwd, '/');
    while (tmp[i])
        i++;
    return (tmp[i - 1]);
}
