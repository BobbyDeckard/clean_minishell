/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:33:01 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 21:26:14 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	exec_minishell(t_ast *node, t_cmd cmd);
int		is_lone_redir_node(t_ast *node);

void	exec_cmd(t_ast *node, t_cmd cmd)
{
	if (!cmd.path || is_lone_redir_node(node))
		return ;
	if (!ft_strncmp(cmd.args[0], "minishell", 10)
		|| ! ft_strncmp(cmd.args[0], "./minishell", 12))
		return (exec_minishell(node, cmd));
	if (execve(cmd.path, cmd.args, node->shell->envp) == -1)
		perror("execve");
}
