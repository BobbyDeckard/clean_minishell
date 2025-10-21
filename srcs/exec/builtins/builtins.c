/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:14:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/10 16:37:11 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	close_all_redirs(t_ast *node);
void	close_redirs(t_cmd *cmd);
void	unlink_heredoc(t_ast *node);
int		cd(t_ast *node, int in_pipe);
int		echo(t_ast *node, int in_pipe);
int		exit_bltn(t_ast *node, int in_pipe);
int		export_bltn(t_ast *node, int in_pipe);
int		has_equal(const char *str);
int		make_redirs(t_ast *node);
int		set_exit_status(t_ast *node, int status);
int		unset(t_ast *node, int in_pipe);

static int	env(t_ast *node, int in_pipe)
{
	char	**envp;
	int		i;

	if (!in_pipe && make_redirs(node))
		return (set_exit_status(node, 1));
	envp = node->shell->envp;
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
