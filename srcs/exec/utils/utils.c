/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:33:50 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/27 20:03:39 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	dup_fds(t_ast node)
{
	if (node.cmd.fd_in != STDIN_FILENO && node.cmd.fd_in >= 0)
	{
		if (dup2(node.cmd.fd_in, STDIN_FILENO) == -1)
		{
			ft_putstr_fd("Failed to dup2 stdin for ", 1);
			ft_putstr_fd(node.cmd.args[0], 1);
			ft_putchar_fd(' ', 1);
			ft_putstr_fd(node.cmd.args[1], 1);
			ft_putchar_fd('\n', 1);
			dup2_error();
		}
		close(node.cmd.fd_in);
	}
	if (node.cmd.fd_out != STDOUT_FILENO && node.cmd.fd_out >= 0)
	{
		if (dup2(node.cmd.fd_out, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("Failed to dup2 stdout for ", 1);
			ft_putstr_fd(node.cmd.args[0], 1);
			ft_putchar_fd(' ', 1);
			ft_putstr_fd(node.cmd.args[1], 1);
			ft_putchar_fd('\n', 1);
			dup2_error();
		}
		close(node.cmd.fd_out);
	}
}

int	set_exit_status(t_ast *node, int status)
{
	node->data->exit_status = status;
	return (status);
}
