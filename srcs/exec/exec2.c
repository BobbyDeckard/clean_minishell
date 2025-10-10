/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:33:01 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/10 17:03:17 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	exec_cmd(t_ast *node, t_cmd cmd)
{
	if (!cmd.path || is_lone_redir(node))
		return ;
	if (!ft_strncmp(cmd.args[0], "minishell", 10)
		|| ! ft_strncmp(cmd.args[0], "./minishell", 12))
		return (exec_minishell(node, cmd));
	if (execve(cmd.path, cmd.args, node->root->data->envp) == -1)
		perror("execve");
}
