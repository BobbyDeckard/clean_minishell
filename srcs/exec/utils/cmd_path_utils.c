/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:18:20 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/28 16:18:39 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_lone_redir_node(t_ast *node);

void	cmd_not_found(t_ast *node, char *name)
{
	if (!is_lone_redir_node(node))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": command not found\n", 2);
	}
}

void	cmd_permission(char *name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": Permission denied\n", 2);
}

void	is_a_dir(char *name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": Is a directory\n", 2);
}

int	is_dir(char *path)
{
	struct stat	buf;

	if (stat(path, &buf))
		perror("stat");
	if (S_ISDIR(buf.st_mode))
		return (1);
	return (0);
}