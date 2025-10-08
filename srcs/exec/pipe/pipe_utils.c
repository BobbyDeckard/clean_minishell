/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:04:42 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/30 17:08:17 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	close_all_redirs(t_ast *node)
{
	close_redirs(&node->cmd);
	unlink_heredoc(node);
}

// For cmd cat file | grep foo:
// 1 pipe is created: fd[0]
// cat->fd_out = fd[0][1] (4)
// grep->fd_in = fd[0][0] (3)
// Main process calls this func and closes fd[0][1] (cat->fd_out)
// 3 is still open in main process (= fd[0][0], grep->fd_in)
// cat's process calls this func and closes fd[0][1] (cat->fd_out)
// 3 is still open in cat's process (grep->fd_in)
// After making fork for grep in main process, this func is called
// and closes fd[0][0] (grep->fd_in)
// main process only has stdfds open
// grep's process calls this func and closes fd[0][0] (grep->fd_in)
// all processes have exited
// The only thing we could do differently seems to be closing 3
// in cat's process
void	close_pipes(int fd[2][2], int i, int count)
{
	fprintf(stderr, "pid = %d\tIn close_pipes()\n", getpid());
	if (i + 1 < count)
	{
//		if (close(fd[i % 2][1]) == -1)
//			perror("close");
//		fprintf(stderr, "pid = %d\tClosed fd[%d][1]\n", getpid(), i % 2);
		fprintf(stderr, "pid = %d\tAbout to close fd[%d][0]\n", getpid(), i % 2);
		if (close(fd[i % 2][0]) == -1)
			perror("close");
	}
//	if (i > 0)
//	{
//		if (close(fd[(i + 1) % 2][0]) == -1)
//			perror("close");
//		fprintf(stderr, "pid = %d\tClosed fd[%d][0]\n", getpid(), (i + 1) % 2);
//		fprintf(stderr, "pid = %d\tAbout to close fd[%d][1]\n", getpid(), (i + 1) % 2);
//		if (close(fd[(i + 1) % 2][1]) == -1)
//			perror("close");
//	}
	fprintf(stderr, "\n");
}

int	make_fork(void)
{
	int	pid;

	pid = fork();
	if (pid < 0)
		fork_error();
	return (pid);
}

int	make_pipe(int fd[2])
{
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	return (1);
}

int	waitpids(t_ast *node, int *pids, int cmd_count)
{
	int	status;
	int	i;

	status = -1;
	i = -1;
	while (++i < cmd_count)
	{
		if (pids[i] < 0)
			status = 1;
		else
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
		}
		node->data->exit_status = status;
	}
	return (status);
}
