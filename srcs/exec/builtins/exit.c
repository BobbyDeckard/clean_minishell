/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:19:07 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/10 16:13:58 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	cleanup(t_ast *node);
void	close_all_redirs(t_ast *node);
int		make_redirs(t_ast *node);
int		set_exit_status(t_ast *node, int status);

static void	check_digits(t_ast *node, char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			cleanup(node);
			exit(2);
		}
	}
}

static void	check_length(t_ast *node, char *arg)
{
	while (*arg == '0')
		arg++;
	if (ft_strlen(arg) > 14)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup(node);
		exit(2);
	}
}

static int	too_many_args(t_ast *node, int in_pipe)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	if (!in_pipe)
		close_all_redirs(node);
	return (set_exit_status(node, 1));
}

int	exit_bltn(t_ast *node, int in_pipe)
{
	int	n;

	ft_putstr_fd("exit\n", node->cmd.fd_out);
	if (!in_pipe && make_redirs(node))
		return (set_exit_status(node, 1));
	if (node->cmd.args[1])
	{
		check_digits(node, node->cmd.args[1]);
		check_length(node, node->cmd.args[1]);
		if (node->cmd.args[2])
			return (too_many_args(node, in_pipe));
		n = ft_atol(node->cmd.args[1]) % 256;
		cleanup(node);
		exit (n);
	}
	else
	{
		n = node->shell->exit_status % 256;
		cleanup(node);
		exit(n);
	}
}
