/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 18:29:02 by tpereira          #+#    #+#             */
/*   Updated: 2022/10/25 18:29:13 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_find_cmd(t_command *cmd)					// needs to free return value!!!!
{
	char	*path;
	char	*cmd_path;
	char	*temp_path;
	char	**all_paths;
	int		i;

	i = 0;
	path = getenv("PATH");
	all_paths = ft_split(path, ':');
	while (all_paths[i])
	{
		temp_path = ft_strjoin(all_paths[i], "/");
		cmd_path = ft_strjoin(temp_path, cmd->argv[0]);
		if (!access(cmd_path, X_OK))
		{
			free(temp_path);
			break ;
		}
		free(cmd_path);
		free(temp_path);
		i++;
	}
	return (cmd_path);
}
