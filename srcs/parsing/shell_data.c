/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:36:45 by imeulema          #+#    #+#             */
/*   Updated: 2025/08/27 21:25:21 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

int	clean_data(t_shell data)
{
	int	i;

	if (data.envp)
	{
		i = -1;
		while (data.envp[++i])
			free(data.envp[i]);
		free(data.envp);
	}
	if (data.paths)
	{
		i = -1;
		while (data.paths[++i])
			free(data.paths[i]);
		free(data.paths);
	}
	return (data.exit_status);
}

//	In case of error within the scope of this function,
//	free everything that's been malloc'ed in this scope,
//	then exit.
t_shell	init_shell_data(char **envp)
{
	t_shell	data;

	data.envp = copy_env(envp);
	data.paths = NULL;
	get_paths(&data);
	data.exit_status = 0;
	data.state = INTERACTIVE;
	return (data);
}
