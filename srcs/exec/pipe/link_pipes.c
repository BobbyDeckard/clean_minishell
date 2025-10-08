/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:26:58 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 15:12:08 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"
void	link_pipe(t_ast *cmd1, t_ast *cmd2, int fd[2][2], int i)
{
	if (cmd1->type == NODE_CMD)
	{
		fprintf(stderr, "%d\tSetting %s->fd_out to fd[%d][1] (%d)\n", getpid(), cmd1->cmd.args[0], i % 2, fd[i % 2][1]);
		cmd1->cmd.fd_out = fd[i % 2][1];
	}
	if (cmd2->type == NODE_CMD)
	{
		fprintf(stderr, "%d\tSetting %s->fd_in to fd[%d][0] (%d)\n", getpid(), cmd2->cmd.args[0], i % 2, fd[i % 2][0]);
		cmd2->cmd.fd_in = fd[i % 2][0];
	}
}
