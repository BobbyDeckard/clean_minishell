/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:50:29 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 15:31:22 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

char	**copy_env(char **envp);
char	**get_paths(t_shell *shell);

t_shell	init_shell(char **envp)
{
	t_shell	shell;

	shell.exit_status = 0;
	shell.command = NULL;
	shell.tokens = NULL;
	shell.paths = NULL;
	shell.root = NULL;
	shell.state = INTERACTIVE;
	shell.envp = copy_env(envp);
	if (*envp)
		shell.paths = get_paths(&shell);
	return (shell);
}
