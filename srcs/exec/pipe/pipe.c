/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:51:21 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:26:12 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	exec_pipe_cmd(t_ast *node)
{
//	ft_putstr_fd("Call to exec_pipe_cmd() made for ", 1);
//	ft_putstr_fd(node->cmd.args[0], 1);
//	ft_putchar_fd('\n', 1);
	dup_fds(*node);
	exec_cmd(node, node->cmd);
//	ft_putstr_fd("Something went wrong with exec_cmd() for ", 1);
//	ft_putstr_fd(node->cmd.args[0], 1);
//	ft_putchar_fd('\n', 1);
	clean_exit(node->root, 1);
}

static void	exec_pipe_child(t_ast *node)
{
	int	status;

//	ft_putstr_fd("Call to exec_pipe_child() made\n", 1);
	setup_child_signals();
	status = 1;
	if (node->type == NODE_CMD)
		exec_pipe_cmd(node);
	else
		status = exec_ast(node);
//	ft_putstr_fd("Pipe child is about to exit with status ", 1);
//	ft_putnbr_fd(status, 1);
//	ft_putchar_fd('\n', 1);
	exit(status);
}

static int	make_and_link_pipe(t_ast **child, int fd[2][2], int i, int count)
{
	if (i + 1 < count)
	{
		if (make_pipe(fd[i % 2]))
			link_pipe(child[i], child[i + 1], fd, i);
		else
		{
			close_pipes(fd, i, count);
			return (1);
		}
	}
//	ft_putstr_fd("make_and_link_pipe() about to exit with status 0\n", 1);
	return (0);
}

static int	run_pipe(t_ast **child, int *pids, int count)
{
	int	fd[2][2];
	int	i;

	i = -1;
	while (++i < count)
	{
		if (make_and_link_pipe(child, fd, i, count))
			return (waitpids((*child)->root, pids, count));
		if (child[i]->type == NODE_CMD && is_builtin(child[i]->cmd)
			&& exec_builtin(child[i]))
			pids[i] = -2;
		else if (child[i]->type == NODE_CMD && !is_builtin(child[i]->cmd))
		{
			if (make_redirs(child[i]))
				pids[i] = -2;
			else
				get_cmd_path(child[i], &child[i]->cmd, child[i]->data->paths);
		}
		if (pids[i] != -2)
		{
			pids[i] = make_fork();
//			ft_putstr_fd("Made fork: pids[", 1);
//			ft_putnbr_fd(i, 1);
//			ft_putstr_fd("]\n", 1);
		}
		if (pids[i] == 0)
			exec_pipe_child(child[i]);
		if (child[i]->type == NODE_CMD && !is_builtin(child[i]->cmd))
			close_redirs_and_unlink_heredoc(child[i]);
		close_pipes(fd, i, count);
	}
	return (waitpids((*child)->root, pids, count));
}

int	exec_pipe(t_ast **children)
{
	int	*pids;
	int	status;
	int	count;

	count = count_nodes(children);
	pids = init_pids((*children)->root, count);
	if (!pids)
		return (1);
	status = run_pipe(children, pids, count);
//	printf("run_pipe() exited with status %d\n", status);
	free(pids);
	return (status);
}
