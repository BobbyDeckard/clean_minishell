/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:44:46 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/08 10:01:45 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

int	is_lone_redir(t_ast *node)
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
