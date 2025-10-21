/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:44:46 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/20 21:26:48 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	cleanup(t_ast *node);
void	expander(t_ast *node, t_cmd *cmd);
void	get_cmd_path(t_ast *node, t_cmd *cmd, char **paths);
int		is_builtin(t_cmd cmd);
int		make_redirs(t_ast *node);

int	is_lone_redir_node(t_ast *node)
{
	if (node->cmd.fd_in == STDIN_FILENO && node->cmd.fd_out == STDOUT_FILENO)
		return (0);
	else if (node->cmd.fd_in < 0 || node->cmd.fd_out < 0)
		return (0);
	else if (node->cmd.args[0])
		return (0);
	return (1);
}

void	prep_cmd(t_ast *node)
{
	expander(node, &node->cmd);
	if (!is_builtin(node->cmd) && !make_redirs(node))
		get_cmd_path(node, &node->cmd, node->shell->paths);
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
		malloc_error(root, root->shell, NULL);
	i = -1;
	while (++i < count)
		pids[i] = -1;
	return (pids);
}

void	setup_child_signals(t_ast *node)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("child sigaction SIGINT");
		cleanup(node);
		exit(1);
	}
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("child sigaction SIGQUIT");
		cleanup(node);
		exit(1);
	}
}
