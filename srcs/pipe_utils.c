/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 11:24:21 by tpereira          #+#    #+#             */
/*   Updated: 2022/12/28 09:49:02 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// t_pipeline *parse_pipeline(char *input, char **envp)
// {
// 	char		*temp;
// 	char		*current;
// 	int			num_cmds;
// 	t_pipeline	*pipeline;

// 	num_cmds = 0;
// 	temp = ft_strdup(input);
// 	current = temp;
// 	while(*current)
// 	{
// 		if (*current == '|')
// 			num_cmds++;
// 		current++;
// 	}
// 	num_cmds++;
// 	pipeline = ft_calloc(sizeof(t_pipeline) + num_cmds * sizeof(t_command *), 1);
// 	pipeline->num_cmds = num_cmds;
// 	pipeline->envp = envp;
// 	return (pipeline);
// }