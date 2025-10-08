/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:04:42 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 16:07:04 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	close_all_redirs(t_ast *node)
{
	close_redirs(&node->cmd);
	unlink_heredoc(node);
}

void	close_pipes(int fd[2][2], int i, int count)
{
	if (i + 1 < count)
	{
		if (close(fd[i % 2][0]) == -1)
			perror("close");
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
