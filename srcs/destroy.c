/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 16:55:43 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/11 14:28:29 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    ft_destroy_cmd(t_command *cmd)
{
	ft_free_split(cmd->argv);
	free(cmd->path);
}

void    ft_free_cmd(t_command *cmd)
{
	ft_destroy_cmd(cmd);
}