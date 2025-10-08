/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:33:50 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 16:07:14 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	invalid_name(t_cmd *cmd, char *name)
{
	cmd->path = NULL;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void	dup_fds(t_ast *node)
{
	if (node->cmd.fd_in != STDIN_FILENO && node->cmd.fd_in >= 0)
	{
		if (dup2(node->cmd.fd_in, STDIN_FILENO) == -1)
			dup2_error(node);
		if (close(node->cmd.fd_in) == -1)
			perror("close");
	}
	if (node->cmd.fd_out != STDOUT_FILENO && node->cmd.fd_out >= 0)
	{
		if (dup2(node->cmd.fd_out, STDOUT_FILENO) == -1)
			dup2_error(node);
		if (close(node->cmd.fd_out) == -1)
			perror("close");
	}
}

int	set_exit_status(t_ast *node, int status)
{
	node->data->exit_status = status;
	return (status);
}
