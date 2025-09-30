/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:33:50 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 17:09:34 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	dup_fds(t_ast node)
{
	if (node.cmd.fd_in != STDIN_FILENO && node.cmd.fd_in >= 0)
	{
		if (dup2(node.cmd.fd_in, STDIN_FILENO) == -1)
			dup2_error();
		close(node.cmd.fd_in);
	}
	if (node.cmd.fd_out != STDOUT_FILENO && node.cmd.fd_out >= 0)
	{
		if (dup2(node.cmd.fd_out, STDOUT_FILENO) == -1)
			dup2_error();
		close(node.cmd.fd_out);
	}
}

int	set_exit_status(t_ast *node, int status)
{
	node->data->exit_status = status;
	return (status);
}
