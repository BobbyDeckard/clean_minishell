/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 21:21:24 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/27 21:29:56 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

static void	get_paths_error(t_shell data)
{
	clean_data(data);
	exit(1);
}

void	get_paths(t_shell *data)
{
	char	**paths;
	char	*all_paths;

	all_paths = getenv("PATH");
	if (!all_paths)
		get_paths_error(*data);
	paths = ft_split_paths(all_paths, ':');
	if (!paths)
		get_paths_error(*data);
	return (paths);
}
