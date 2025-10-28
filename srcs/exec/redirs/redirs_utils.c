/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:39:49 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/21 09:57:14 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	redir_expander(t_ast *node, t_rdr *rdr);
void	unlink_heredoc(t_ast *node);

void	make_file_name(t_ast *node)
{
	char	*file;
	int		len;
	int		i;

	redir_expander(node, &node->rdr);
	if (!node->rdr.args[0])
		return ;
	len = 1;
	i = -1;
	while (node->rdr.args[++i])
		len += ft_strlen(node->rdr.args[i]);
	file = (char *) malloc(len * sizeof(char));
	if (!file)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(file, node->rdr.args[0], len);
	i = 0;
	while (node->rdr.args[++i])
		ft_strlcat(file, node->rdr.args[i], len);
	node->rdr.file = file;
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
