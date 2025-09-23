/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:42:40 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:27:02 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	found_path(t_cmd *cmd, char *path)
{
	cmd->path = ft_strdup(path);	// doesn't it work just doing cmd->path = path ?
	free(path);
}

void	get_cmd_path(t_cmd *cmd, char **paths)
{
	char	*full_path;
	char	*name;
	int		i;

	name = cmd->args[0];
	i = -1;
	while (paths && paths[++i])
	{
		full_path = ft_strjoin(paths[i], name);
		if (!access(full_path, F_OK))
			return (found_path(cmd, full_path));
		free(full_path);
	}
	cmd->path = NULL;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": command not found\n", 2);
}
