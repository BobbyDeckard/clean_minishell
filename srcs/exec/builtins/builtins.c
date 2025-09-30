/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:14:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/23 23:29:15 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static int	env(t_ast *node)
{
	char	**envp;
	int		i;

	if (make_redirs(node))
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
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, 0));
}

static int	pwd(t_ast *node)
{
	char	*cwd;

	if (make_redirs(node))
		return (set_exit_status(node, 1));
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		close_redirs(node->cmd);
		unlink_heredoc(node);
		return (set_exit_status(node, 1));
	}
	ft_putstr_fd(cwd, node->cmd.fd_out);
	ft_putchar_fd('\n', node->cmd.fd_out),
	free(cwd);
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, 0));
}

static int	echo(t_ast *node)
{
	int	flag;
	int	i;

	if (make_redirs(node))
		return (set_exit_status(node, 1));
	flag = 0;
	if (!ft_strncmp(node->cmd.args[1], "-n", node->cmd.fd_out))
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
	close_redirs(node->cmd);
	unlink_heredoc(node);
	return (set_exit_status(node, 0));
}

int	exec_builtin(t_ast *node)
{
	char	*name;

	if (!node->cmd.args)
		return (1);
	name = node->cmd.args[0];
	if (!ft_strncmp(name, "echo", 5))
		return (echo(node));
	else if (!ft_strncmp(name, "cd", 3))
		return (cd(node));
	else if (!ft_strncmp(name, "pwd", 4))
		return (pwd(node));
	else if (!ft_strncmp(name, "export", 7))
		return (export_bltn(node));
	else if (!ft_strncmp(name, "unset", 6))
		return (unset(node));
	else if (!ft_strncmp(name, "env", 4))
		return (env(node));
	else if (!ft_strncmp(name, "exit", 5))
		return (exit_bltn(node), 1);
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
