/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 21:21:24 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/11 15:57:18 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

char	**ft_split_paths(t_shell *shell, const char *s, char c);
int		clean_shell(t_shell *shell);

void	update_paths(t_ast *node, t_shell *shell, const char *arg)
{
	char	**paths;
	int		i;

	paths = ft_split_paths(shell, arg, ':');
	if (!paths)
		malloc_error(node, shell, NULL);
	if (shell->paths)
	{
		i = -1;
		while (shell->paths && shell->paths[++i])
		{
			free(shell->paths[i]);
			shell->paths[i] = NULL;
		}
		free(shell->paths);
	}
	shell->paths = paths;
}

char	**get_paths(t_shell *shell)
{
	char	**paths;
	char	*all_paths;

	all_paths = getenv("PATH");
	if (!all_paths)
	{
		perror("getenv");
		clean_shell(shell);
		return (NULL);
	}
	paths = ft_split_paths(shell, all_paths, ':');
	return (paths);
}
