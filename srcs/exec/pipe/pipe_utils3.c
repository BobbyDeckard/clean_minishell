/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 12:22:18 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/03 12:25:17 by imeulema         ###   ########.fr       */
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
