/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:44:46 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/03 14:15:05 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

/*
int	exec_pipe_and(t_ast *node)
{
	int	status;
	int	pid;
	int	i;

	i = -1;
	status = 1;
	while (node->children[++i])
	{
		if (node->children[i]->type == NODE_CMD)
		{
			prep_cmd(node->children[i]);
			pid = make_fork();
			if (pid == 0)
				exec_pipe_child(node->children[i]);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			if (status)
				break ;
		}
		else
			exec_pipe_child(node->children[i]);
	}
	return (status);
}

int	exec_pipe_or(t_ast *node)
{
	int	status;
	int	pid;
	int	i;

	i = -1;
	status = 1;
	while (node->children[++i])
	{
		if (node->children[i]->type == NODE_CMD)
		{
			prep_cmd(node->children[i]);
			pid = make_fork();
			if (pid == 0)
				exec_pipe_child(node->children[i]);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			if (!status)
				break ;
		}
		else
			exec_pipe_child(node->children[i]);
	}
	return (status);
}
*/
void	prep_cmd(t_ast *node)
{
	expander(node, &node->cmd);
	if (!is_builtin(node->cmd) && !make_redirs(node))
		get_cmd_path(node, &node->cmd, node->data->paths);
}

int	count_nodes(t_ast **children)
{
	int	i;

	i = 0;
	while (children[i])
		i++;
	return (i);
}

int	*init_pids(t_ast *root, int count)
{
	int	*pids;
	int	i;

	pids = (int *) malloc(count * sizeof(int));
	if (!pids)
		malloc_error(root, root->data, NULL);
	i = -1;
	while (++i < count)
		pids[i] = -1;
	return (pids);
}
