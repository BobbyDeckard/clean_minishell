/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:26:58 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 17:08:55 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"
void	link_pipe(t_ast *cmd1, t_ast *cmd2, int fd[2][2], int i)
{
	if (cmd1->type == NODE_CMD)
		cmd1->cmd.fd_out = fd[i % 2][1];
	if (cmd2->type == NODE_CMD)
		cmd2->cmd.fd_in = fd[i % 2][0];
}
