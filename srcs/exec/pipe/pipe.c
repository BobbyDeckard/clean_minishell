/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:51:21 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 16:07:55 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
static void	print_open_fds(const char *name)
{
	fprintf(stderr, "Printing open fds for %s\n", name);
	for (int fd = 0; fd < 1024; fd++)
    	if (fcntl(fd, F_GETFD) != -1)
        	fprintf(stderr, "%d\thas fd %d open\n", getpid(), fd);
}
*/

static void	exec_pipe_cmd(t_ast *node, int fd[2][2], int i, int count)
{
	if (is_lone_redir(node))
		clean_exit(node->root, 0);
	dup_fds(node);
	close_pipes(fd, i, count);
	exec_cmd(node, node->cmd);
	clean_exit(node->root, 1);
}

void	exec_pipe_child(t_ast *node, int fd[2][2], int i, int count)
{
	int	status;

	setup_child_signals(node);
	status = 1;
	if (node->type == NODE_CMD && !is_builtin(node->cmd))
		exec_pipe_cmd(node, fd, i, count);
	else if (node->type == NODE_CMD)
	{
		dup_fds(node);
		close_pipes(fd, i, count);
		status = exec_builtin(node, 1);
	}
	cleanup(node);
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
			return (waitpids(*child, pids, count));
		if (child[i]->type == NODE_CMD)
			prep_cmd(child[i]);
		pids[i] = make_fork();
		if (pids[i] == 0)
			exec_pipe_child(child[i], fd, i, count);
		if (child[i]->type == NODE_CMD)
			close_all_redirs(child[i]);
	}
	return (waitpids(*child, pids, count));
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
	free(pids);
	return (status);
}
