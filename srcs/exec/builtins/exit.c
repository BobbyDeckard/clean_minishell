/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:19:07 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/03 15:35:07 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

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
			exit(255);
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
		exit(255);
	}
}

int	exit_bltn(t_ast *node)
{
	int	n;

	ft_putstr_fd("exit\n", node->cmd.fd_out);
	if (make_redirs(node))
		return (set_exit_status(node, 1));
	if (node->cmd.args[2])
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	else if (node->cmd.args[1])
	{
		check_digits(node, node->cmd.args[1]);
		check_length(node, node->cmd.args[1]);
		n = ft_atol(node->cmd.args[1]) % 256;
		close_all_redirs(node);
		cleanup(node);
		exit(n);
	}
	else
	{
		n = node->data->exit_status;
		close_all_redirs(node);
		cleanup(node);
		exit(n);
	}
	return (1);
}
