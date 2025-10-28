/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:51:21 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 21:26:30 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	cleanup(t_ast *node);
void	clean_exit(t_ast *node, int status);
void	close_all_redirs(t_ast *node);
void	close_pipes(int fd[2][2], int i, int count);
void	dup_fds(t_ast *node);
void	exec_cmd(t_ast *node, t_cmd cmd);
void	link_pipe(t_ast *cmd1, t_ast *cmd2, int fd[2][2], int i);
void	prep_cmd(t_ast *node);
void	setup_child_signals(t_ast *node);
int		*init_pids(t_ast *root, int count);
int		count_nodes(t_ast **children);
int		exec_builtin(t_ast *node, int in_pipe);
int		is_builtin(t_cmd cmd);
int		is_lone_redir_node(t_ast *node);
int		make_fork(void);
int		make_pipe(int fd[2]);
int		waitpids(t_ast *node, int *pids, int cmd_count);

static void	print_open_fds(const char *name)
{
	fprintf(stderr, "%d\tPrinting open fds for %s\n", getpid(), name);
	for (int fd = 0; fd < 1024; fd++)
    	if (fcntl(fd, F_GETFD) != -1)
        	fprintf(stderr, "%d\thas fd %d open\n", getpid(), fd);
	fprintf(stderr, "\n");
}

static void	exec_pipe_cmd(t_ast **child, int fd[2][2], int i, int count)
{
	int	fd_in;

	if (is_lone_redir_node(child[i]))
		clean_exit(child[i]->root, 0);
	dup_fds(child[i]);
	fprintf(stderr, "%d\tOpen fds after dup call:\n", getpid());
	print_open_fds(child[i]->cmd.args[0]);
	close_pipes(fd, i, count);
	fprintf(stderr, "%d\tOpen fds after close_pipes call:\n", getpid());
	print_open_fds(child[i]->cmd.args[0]);
	exec_cmd(child[i], child[i]->cmd);
	if (child[i + 1])
	{
		fd_in = child[i + 1]->cmd.fd_in;
		fprintf(stderr, "%d\tAttempting manual cleanup.\n\tfd_in set to: %d\n", getpid(), fd_in);
		if (fd_in != STDIN_FILENO && fd_in > 0)
		{
//			char *str = (char *) malloc(512);
//			str = ft_itoa(getpid());
//			ft_strlcat(str, "\tclose", 512);
//			if (close(fd_in))
//				perror(str);
//			free(str);
			child[i + 1]->cmd.fd_in = STDIN_FILENO;
		}
	}
	fprintf(stderr, "%d\tOpen fds before call to clean_exit:\n", getpid());
	print_open_fds(child[i]->cmd.args[0]);
	clean_exit(child[i]->root, 1);
}

void	exec_pipe_child(t_ast **child, int fd[2][2], int i, int count)
{
	int	status;

	setup_child_signals(child[i]);
	status = 1;
	fprintf(stderr, "%d\tOpen fds before dup call:\n", getpid());
	print_open_fds(child[i]->cmd.args[0]);
	if (child[i]->type == NODE_CMD && !is_builtin(child[i]->cmd))
		exec_pipe_cmd(child, fd, i, count);
	else if (child[i]->type == NODE_CMD)
	{
		dup_fds(child[i]);
		fprintf(stderr, "%d\tOpen fds after dup call:\n", getpid());
		status = exec_builtin(child[i], 1);
	}
	cleanup(child[i]);
	fprintf(stderr, "%d\tOpen fds after cleanup:\n", getpid());
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
		fprintf(stderr, "%d\tPipe run %d about to fork. Open fds:\n", getpid(), i);
		print_open_fds("main");
		pids[i] = make_fork();
		if (pids[i] == 0)
		{
			free(pids);
			exec_pipe_child(child, fd, i, count);
		}
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
