/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_data.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:36:45 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/29 13:35:42 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

t_shell	init_shell_data(char **envp)
{
	t_shell	data;

	data.tokens = NULL;
	data.envp = copy_env(envp);
	data.paths = NULL;
	data.cmd = NULL;
	if (*envp)
		get_paths(&data);
	data.exit_status = 0;
	data.state = INTERACTIVE;
	return (data);
}
