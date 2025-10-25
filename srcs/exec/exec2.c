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

void	close_redirs(t_cmd *cmd);
void	exec_minishell(t_ast *node, t_cmd cmd);
void	unlink_heredoc(t_ast *node);
int		make_redirs(t_ast *node);
int		is_lone_redir_node(t_ast *node);
int		set_exit_status(t_ast *node, int status);

int	exec_solo_redirs(t_ast *node)
{
	if (make_redirs(node))
		return (set_exit_status(node, 1));
	close_redirs(&node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, 0));
}

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
