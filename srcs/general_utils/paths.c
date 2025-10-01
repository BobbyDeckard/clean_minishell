/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 21:21:24 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/29 13:35:23 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	update_paths(t_ast *node, t_shell *data, const char *arg)
{
	char	**paths;
	int		i;

	paths = ft_split_paths(data, arg, ':');
	if (!paths)
		malloc_error(node, data, NULL);
	if (data->paths)
	{
		i = -1;
		while (data->paths && data->paths[++i])
		{
			free(data->paths[i]);
			data->paths[i] = NULL;
		}
		free(data->paths);
	}
	data->paths = paths;
}

void	get_paths(t_shell *data)
{
	char	**paths;
	char	*all_paths;

	all_paths = getenv("PATH");
	paths = ft_split_paths(data, all_paths, ':');
	data->paths = paths;
}
