/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:14:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/09 13:09:56 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	env(t_ast *node, int in_pipe)
{
	char	**envp;
	int		i;

	if (!in_pipe && make_redirs(node))
		return (set_exit_status(node, 1));
	envp = node->data->envp;
	i = -1;
	while (envp[++i])
	{
		if (!has_equal(envp[i]))
			continue ;
		ft_putstr_fd(envp[i], node->cmd.fd_out);
		ft_putchar_fd('\n', node->cmd.fd_out);
	}
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 0));
}

static int	pwd(t_ast *node, int in_pipe)
{
	char	*cwd;

	if (!in_pipe && make_redirs(node))
		return (set_exit_status(node, 1));
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		close_redirs(&node->cmd);
		unlink_heredoc(node);
		return (set_exit_status(node, 1));
	}
	ft_putstr_fd(cwd, node->cmd.fd_out);
	ft_putchar_fd('\n', node->cmd.fd_out),
	free(cwd);
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 0));
}

static int	echo(t_ast *node, int in_pipe)
{
	int	flag;
	int	i;

	if (!in_pipe && make_redirs(node))
		return (set_exit_status(node, 1));
	flag = 0;
	if (!ft_strncmp(node->cmd.args[1], "-n", 2))
		flag++;
	i = flag;
	while (node->cmd.args[++i])
	{
		ft_putstr_fd(node->cmd.args[i], node->cmd.fd_out);
		if (node->cmd.args[i + 1])
			ft_putchar_fd(' ', node->cmd.fd_out);
	}
	if (!flag)
		ft_putchar_fd('\n', node->cmd.fd_out);
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 0));
}

int	exec_builtin(t_ast *node, int in_pipe)
{
	char	*name;

	if (!node->cmd.args)
		return (1);
	name = node->cmd.args[0];
	if (!ft_strncmp(name, "echo", 5))
		return (echo(node, in_pipe));
	else if (!ft_strncmp(name, "cd", 3))
		return (cd(node, in_pipe));
	else if (!ft_strncmp(name, "pwd", 4))
		return (pwd(node, in_pipe));
	else if (!ft_strncmp(name, "export", 7))
		return (export_bltn(node, in_pipe));
	else if (!ft_strncmp(name, "unset", 6))
		return (unset(node, in_pipe));
	else if (!ft_strncmp(name, "env", 4))
		return (env(node, in_pipe));
	else if (!ft_strncmp(name, "exit", 5))
		return (exit_bltn(node, in_pipe));
	return (1);
}

int	is_builtin(t_cmd cmd)
{
	char	*name;

	if (!cmd.args)
		return (0);
	name = cmd.args[0];
	if (!ft_strncmp(name, "echo", 5))
		return (1);
	else if (!ft_strncmp(name, "cd", 3))
		return (1);
	else if (!ft_strncmp(name, "pwd", 4))
		return (1);
	else if (!ft_strncmp(name, "export", 7))
		return (1);
	else if (!ft_strncmp(name, "unset", 6))
		return (1);
	else if (!ft_strncmp(name, "env", 4))
		return (1);
	else if (!ft_strncmp(name, "exit", 5))
		return (1);
	return (0);
}
