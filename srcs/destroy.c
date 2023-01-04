/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 16:55:43 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/04 17:01:09 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void    ft_destroy_cmd(t_command *cmd)
{
    free(cmd->cmd);
    ft_free_split(cmd->argv);
    ft_free_split(cmd->envp);
}

void    ft_free_cmd(t_command *cmd)
{
    while (cmd->next)
    {
        ft_destroy_cmd(cmd);
        cmd = cmd->next;
    }
    //ft_destroy_cmd(cmd);
}