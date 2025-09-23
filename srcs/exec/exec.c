/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:54:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/23 22:40:22 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

// Using exit() is necessary since we're in a child process
void	exec_cmd(t_ast *node, t_cmd cmd)
{
	if (!cmd.path)
		exit(1);
	if (execve(cmd.path, cmd.args, node->root->data->envp) == -1)
		perror("execve");
}

static int	run_cmd(t_ast *node)
{
	int	status;
	int	pid;

	status = -1;
	expander(node, &node->cmd);
	if (is_builtin(node->cmd))
		return (exec_builtin(node));
	else if (make_redirs(node))
		return (set_exit_status(node, 1));
	get_cmd_path(node, &node->cmd, node->data->paths);
	pid = fork();
	if (pid < 0)
		return (fork_error());
	else if (pid == 0)
	{
		dup_fds(*node);
		exec_cmd(node, node->cmd);
		clean_exit(node->root, 1);
	}
	close_redirs(node->cmd);
	waitpid(pid, &status, 0);
	unlink_heredoc(node);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (set_exit_status(node, status));
}

static int	exec_or_if(t_ast **children)
{
	int	i;

	i = -1;
	while (children[++i])
	{
		if (!exec_ast(children[i]))
			return (0);
	}
	return (1);
}

static int	exec_and_if(t_ast **children)
{
	int	i;

	i = -1;
	while (children[++i])
	{
		if (exec_ast(children[i]))
			return (1);
	}
	return (0);
}

int	exec_ast(t_ast *node)
{
	if (node->type == NODE_CMD)
		return (run_cmd(node));
	else if (node->type == NODE_AND_IF && node->children)
		return (exec_and_if(node->children));
	else if (node->type == NODE_OR_IF && node->children)
		return (exec_or_if(node->children));
	else if (node->type == NODE_PIPE && node->children)
		return (exec_pipe(node->children));
	return (1);
}
