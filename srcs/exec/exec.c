/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:54:44 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 21:27:59 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incl/minishell.h"

void	clean_exit(t_ast *node, int status);
void	close_redirs(t_cmd *cmd);
void	dup_fds(t_ast *node);
void	exec_cmd(t_ast *node, t_cmd cmd);
void	expander(t_ast *node, t_cmd *cmd);
void	get_cmd_path(t_ast *node, t_cmd *cmd, char **paths);
void	unlink_heredoc(t_ast *node);
int		exec_ast(t_ast *node);
int		exec_builtin(t_ast *node, int in_pipe);
int		exec_pipe(t_ast **children);
int		exec_solo_redirs(t_ast *node);
int		fork_error(void);
int		is_builtin(t_cmd cmd);
int		is_lone_redir_node(t_ast *node);
int		make_redirs(t_ast *node);
int		set_exit_status(t_ast *node, int status);

static void	run_child_process(t_ast *node)
{
	dup_fds(node);
	exec_cmd(node, node->cmd);
	if (is_lone_redir_node(node))
		clean_exit(node->root, 0);
	if (!node->cmd.path)
		clean_exit(node, 127);
	clean_exit(node->root, 1);
}

static int	run_cmd(t_ast *node)
{
	int	status;
	int	pid;

	status = -1;
	expander(node, &node->cmd);
	if (is_builtin(node->cmd))
		return (exec_builtin(node, 0));
	else if (make_redirs(node))
		return (set_exit_status(node, 1));
	get_cmd_path(node, &node->cmd, node->shell->paths);
	pid = fork();
	if (pid < 0)
		return (fork_error());
	else if (pid == 0)
		run_child_process(node);
	close_redirs(&node->cmd);
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
	else if (node->type == NODE_AND && node->children)
		return (exec_and_if(node->children));
	else if (node->type == NODE_OR && node->children)
		return (exec_or_if(node->children));
	else if (node->type == NODE_PIPE && node->children)
		return (exec_pipe(node->children));
	else if (node->type == NODE_SOLO_REDIR && node->children)
		return (exec_solo_redirs(node));
	return (1);
}
