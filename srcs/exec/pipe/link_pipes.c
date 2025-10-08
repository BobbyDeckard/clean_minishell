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
/*
static void	set_fd_out(t_ast *node, int fd[2][2], int i)
{
	int	j;

	if (node->type == NODE_CMD)
		node->cmd.fd_out = fd[i % 2][1];
	else if (node->children)
	{
		j = -1;
		while (node->children[++j])
			set_fd_out(node->children[j], fd, i);
	}
}

static void	set_fd_in(t_ast *node, int fd[2][2], int i)
{
	int	j;

	if (node->type == NODE_CMD)
		node->cmd.fd_in = fd[i % 2][0];
	else if (node->children)
	{
		j = -1;
		while (node->children[++j])
			set_fd_in(node->children[j], fd, i);
	}
}
*/
void	link_pipe(t_ast *cmd1, t_ast *cmd2, int fd[2][2], int i)
{
//	int	j;

	printf("In link_pipe() for %s and %s\n", cmd1->cmd.args[0], cmd2->cmd.args[0]);
	if (cmd1->type == NODE_CMD)
	{
		cmd1->cmd.fd_out = fd[i % 2][1];
		printf("Set fd_out to fd[%d][1] (%d) for %s\n", i % 2, fd[i % 2][1], cmd1->cmd.args[0]);
	}
//	else if (cmd1->children)
//	{
//		j = -1;
//		while (cmd1->children[++j])
//			set_fd_out(cmd1->children[j], fd, i);
//	}
	if (cmd2->type == NODE_CMD)
	{
		cmd2->cmd.fd_in = fd[i % 2][0];
		printf("Set fd_in to fd[%d][0] (%d) for %s\n", i % 2, fd[i % 2][0], cmd2->cmd.args[0]);
	}
	printf("\n");
//	else if (cmd2->children)
//	{
//		j = -1;
//		while (cmd2->children[++j])
//			set_fd_in(cmd2->children[j], fd, i);
//	}
}
