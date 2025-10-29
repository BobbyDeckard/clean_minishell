/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:36:51 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/10 16:38:14 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	close_all_redirs(t_ast *node);
int		make_redirs(t_ast *node);
int		set_exit_status(t_ast *node, int status);

static int	check_flag(const char *str)
{
	int	i;

	i = 1;
	while (str[++i])
	{
		if (str[i] != 'n')
			return (0);
	}
	return (1);
}

int	echo(t_ast *node, int in_pipe)
{
	int	flag;
	int	i;

	if (make_redirs(node))
		return (set_exit_status(node, 1));
	flag = 0;
	if (!ft_strncmp(node->cmd.args[1], "-n", 2))
		flag = check_flag(node->cmd.args[1]);
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
