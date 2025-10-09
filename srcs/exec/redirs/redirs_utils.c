/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:39:49 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/06 15:53:27 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"
#include <unistd.h>

void	make_file_name(t_ast *node)
{
	char	*file;
	int		len;
	int		i;

	expander(node, &node->cmd);
	len = 1;
	i = -1;
	while (node->cmd.args[++i])
		len += ft_strlen(node->cmd.args[i]);
	file = (char *) malloc(len * sizeof(char));
	if (!file)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(file, node->cmd.args[0], len);
	i = 0;
	while (node->cmd.args[++i])
		ft_strlcat(file, node->cmd.args[i], len);
	node->file = file;
}

void	close_redirs(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO && cmd->fd_in >= 0)
	{
		if (close(cmd->fd_in))
			perror("close");
		cmd->fd_in = STDIN_FILENO;
	}
	if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out >= 0)
	{
		if (close(cmd->fd_out))
			perror("close");
		cmd->fd_out = STDOUT_FILENO;
	}
}

int	check_redirs(t_ast *node, t_cmd *cmd)
{
	if (cmd->fd_in < 0 || cmd->fd_out < 0)
	{
		close_redirs(cmd);
		unlink_heredoc(node);
		return (1);
	}
	return (0);
}
