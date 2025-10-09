/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:26:58 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 16:07:22 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	link_pipe(t_ast *cmd1, t_ast *cmd2, int fd[2][2], int i)
{
	if (cmd1->type == NODE_CMD)
	{
		if (cmd1->cmd.fd_out != STDOUT_FILENO)
		{
			if (close(cmd1->cmd.fd_out))
				perror("close");
		}
		cmd1->cmd.fd_out = fd[i % 2][1];
	}
	if (cmd2->type == NODE_CMD)
	{
		if (cmd2->cmd.fd_in != STDIN_FILENO)
		{
			if (close(cmd2->cmd.fd_in))
				perror("close");
		}
		cmd2->cmd.fd_in = fd[i % 2][0];
	}
}
