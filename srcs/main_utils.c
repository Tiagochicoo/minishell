/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 18:29:02 by tpereira          #+#    #+#             */
/*   Updated: 2023/01/06 16:28:35 by tpereira         ###   ########.fr       */
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
	if (ft_str_isalnum(cmd->argv[0]))
	{
		path = getenv("PATH");
		all_paths = ft_split(path, ":");
		while (all_paths[i])
		{
			temp_path = ft_strjoin(all_paths[i], "/");
			cmd_path = ft_strjoin(temp_path, cmd->argv[0]);
			if (!access(cmd_path, X_OK))
			{
				free(temp_path);
				ft_free_split(all_paths);
				return (cmd_path);
			}
			free(cmd_path);
			free(temp_path);
			cmd_path = NULL;
			i++;
		}
		ft_free_split(all_paths);
	}
	else
		cmd_path = NULL;

	return (cmd_path);
}
