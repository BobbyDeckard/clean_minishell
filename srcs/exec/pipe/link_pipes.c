/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:26:58 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:25:56 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	set_fd_out(t_ast *node, int fd[2][2], int i)
{
	int	j;

	if (node->type == NODE_CMD)
	{
		node->cmd.fd_out = fd[i % 2][1];
	}
	else if (node->children)
	{
		j = -1;
		while (node->children[++j])
		{
			set_fd_out(node->children[j], fd, i);
		}
	}
}

static void	set_fd_in(t_ast *node, int fd[2][2], int i)
{
	int	j;

	if (node->type == NODE_CMD)
	{
		node->cmd.fd_in = fd[i % 2][0];
	}
	else if (node->children)
	{
		j = -1;
		while (node->children[++j])
		{
			set_fd_in(node->children[j], fd, i);
		}
	}
}

void	link_pipe(t_ast *cmd1, t_ast *cmd2, int fd[2][2], int i)
{
	int	j;

	if (cmd1->type == NODE_CMD)
	{
//		ft_putstr_fd("Setting fd_out to fd[", 1);
//		ft_putnbr_fd(i % 2, 1);
//		ft_putstr_fd("][1] for cmd ", 1);
//		ft_putstr_fd(cmd1->cmd.args[0], 1);
//		ft_putchar_fd('\n', 1);
		cmd1->cmd.fd_out = fd[i % 2][1];
	}
	else if (cmd1->children)
	{
		j = -1;
		while (cmd1->children[++j])
		{
			set_fd_out(cmd1->children[j], fd, i);
		}
	}
	if (cmd2->type == NODE_CMD)
	{
//		ft_putstr_fd("Setting fd_in to fd[", 1);
//		ft_putnbr_fd(i % 2, 1);
//		ft_putstr_fd("][0] for cmd ", 1);
//		ft_putstr_fd(cmd1->cmd.args[0], 1);
//		ft_putchar_fd('\n', 1);
		cmd2->cmd.fd_in = fd[i % 2][0];
	}
	else if (cmd2->children)
	{
		j = -1;
		while (cmd2->children[++j])
		{
			set_fd_in(cmd2->children[j], fd, i);
		}
	}
}
