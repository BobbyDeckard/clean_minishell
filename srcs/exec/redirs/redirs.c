/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:21:56 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/06 16:54:04 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

static void	redir_error(t_ast *node)
{
	char	*str;
	int		len;

	len = ft_strlen(node->file) + 12;
	str = (char *) malloc(len * sizeof(char));
	if (!str)
		malloc_error(node, node->data, NULL);
	ft_strlcpy(str, "minishell: ", len);
	ft_strlcat(str, node->file, len);
	perror(str);
	free(str);
}

static int	make_redir_in(t_ast *node, t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	if (!node->file)
		make_file_name(node);
	if (access(node->file, F_OK) || access(node->file, R_OK))
		cmd->fd_in = -1;
	else
		cmd->fd_in = open(node->file, O_RDONLY);
	if (cmd->fd_in < 0)
	{
		redir_error(node);
		return (1);
	}
	return (0);
}

static int	make_redir_out(t_ast *node, t_cmd *cmd)
{
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	if (!node->file)
		make_file_name(node);
	if (!access(node->file, F_OK) && access(node->file, W_OK))
		cmd->fd_out = -1;
	else
		cmd->fd_out = open(node->file, O_TRUNC | O_WRONLY | O_CREAT, 0644);
	if (cmd->fd_out < 0)
	{
		redir_error(node);
		return (1);
	}
	return (0);
}

static int	make_redir_append(t_ast *node, t_cmd *cmd)
{
	if (cmd->fd_out != STDOUT_FILENO)
		close(cmd->fd_out);
	if (!node->file)
		make_file_name(node);
	if (!access(node->file, F_OK) && access(node->file, W_OK))
		cmd->fd_out = -1;
	else
		cmd->fd_out = open(node->file, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if (cmd->fd_out < 0)
	{
		redir_error(node);
		return (1);
	}
	return (0);
}

int	make_redirs(t_ast *node)
{
	t_node_type	type;
	int			i;

	i = -1;
	while (node->children && node->children[++i])
	{
		type = node->children[i]->type;
		if (type == NODE_REDIR_IN && make_redir_in(node->children[i], &node->cmd))
			break ;
		else if (type == NODE_REDIR_OUT && make_redir_out(node->children[i], &node->cmd))
				break ;
		else if (type == NODE_REDIR_APPEND && make_redir_append(node->children[i], &node->cmd))
				break ;
		else if ((type == NODE_HEREDOC || type == NODE_HEREDOC_EXP))
			make_heredoc(node->children[i], &node->cmd);
	}
	return (check_redirs(node, &node->cmd));
}
