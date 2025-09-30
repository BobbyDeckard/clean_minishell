/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:19:07 by imeulema          #+#    #+#             */
/*   Updated: 2025/09/27 20:05:03 by imeulema         ###   ########.fr       */
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

void	exit_bltn(t_ast *node)
{
	int	n;

	ft_putstr_fd("exit\n", node->cmd.fd_out);
	if (node->cmd.args[2])
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	else if (node->cmd.args[1])
	{
		check_digits(node, node->cmd.args[1]);
		n = ft_atoi(node->cmd.args[1]);
		cleanup(node);
		exit(n);
	}
	else
	{
		n = node->data->exit_status;
		cleanup(node);
		exit(n);
	}
}
