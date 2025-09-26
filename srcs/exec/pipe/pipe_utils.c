/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:04:42 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/12 15:26:19 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	close_redirs_and_unlink_heredoc(t_ast *node)
{
	close_redirs(node->cmd);
	unlink_heredoc(node);
}

void	close_pipes(int fd[2][2], int i, int count)
{
	if (i + 1 < count)
	{
		close(fd[i % 2][1]);
//		ft_putstr_fd("Closed fd[", 1);
//		ft_putnbr_fd(i % 2, 1);
//		ft_putstr_fd("][1]\n", 1);
	}
	if (i > 0)
	{
		close(fd[(i + 1) % 2][0]);
//		ft_putstr_fd("Closed fd[", 1);
//		ft_putnbr_fd((i + 1) % 2, 1);
//		ft_putstr_fd("][0]\n", 1);
	}
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

int	waitpids(t_ast *root, int *pids, int cmd_count)
{
	int	status;
	int	i;

	status = -1;
	i = -1;
	while (++i < cmd_count)
	{
		if (pids[i] < -1)
			status = 1;
		else if (pids[i] == -1)
			status = 0;
		else
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
		}
		root->exit_status = status;
	}
	return (status);
}
