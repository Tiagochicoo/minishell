/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:12:21 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/04 16:51:01 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char    *ft_relative_path(char *cwd)
{
    char    **tmp;
    char    *path;
    int     i;

    i = 0;
    tmp = ft_split(cwd, "/");
    while (tmp[i])
        i++;
    path = ft_strdup(tmp[i - 1]);
    ft_free_split(tmp);
    return (path);
}
