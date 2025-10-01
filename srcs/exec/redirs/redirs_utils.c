/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:39:49 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:26:48 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"
#include <unistd.h>

void	close_redirs(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO && cmd->fd_in >= 0)
	{
		close(cmd->fd_in);
		cmd->fd_in = STDIN_FILENO;
	}
	if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out >= 0)
	{
		close(cmd->fd_out);
		cmd->fd_out = STDOUT_FILENO;
	}
}

int	check_redirs(t_ast *node, t_cmd cmd)
{
	if (cmd.fd_in < 0 || cmd.fd_out < 0)
	{
		close_redirs(cmd);
		unlink_heredoc(node);
		return (1);
	}
	return (0);
}
