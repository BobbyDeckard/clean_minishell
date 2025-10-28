/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imeulema <imeulema@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:21:56 by imeulema          #+#    #+#             */
/*   Updated: 2025/10/27 20:22:36 by imeulema         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incl/minishell.h"

void	make_file_name(t_ast *node);
void	make_heredoc(t_ast *node, t_cmd *cmd);
int		check_redirs(t_ast *node, t_cmd *cmd);

static void	redir_error(t_ast *node)
{
	char	*str;
	int		len;

	if (node->rdr.file)
		len = ft_strlen(node->rdr.file) + 12;
	else
		return ;
	str = (char *) malloc(len * sizeof(char));
	if (!str)
		malloc_error(node, node->shell, NULL);
	ft_strlcpy(str, "minishell: ", len);
	ft_strlcat(str, node->rdr.file, len);
	perror(str);
	free(str);
}

static int	make_redir_in(t_ast *node, t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (close(cmd->fd_in))
			perror("close");
	}
	make_file_name(node);
	if (!node->rdr.file)
	{
		cmd->fd_out = -1;
		return (1);
	}
	else if (access(node->rdr.file, F_OK) || access(node->rdr.file, R_OK))
		cmd->fd_in = -1;
	else
		cmd->fd_in = open(node->rdr.file, O_RDONLY);
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
	{
		if (close(cmd->fd_out))
			perror("close");
	}
	make_file_name(node);
	if (!node->rdr.file)
	{
		cmd->fd_out = -1;
		return (1);
	}
	if (!access(node->rdr.file, F_OK) && access(node->rdr.file, W_OK))
		cmd->fd_out = -1;
	else
		cmd->fd_out = open(node->rdr.file, O_TRUNC | O_WRONLY | O_CREAT, 0644);
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
	{
		if (close(cmd->fd_out))
			perror("close");
	}
	make_file_name(node);
	if (!node->rdr.file)
	{
		cmd->fd_out = -1;
		return (1);
	}
	else if (!access(node->rdr.file, F_OK) && access(node->rdr.file, W_OK))
		cmd->fd_out = -1;
	else
		cmd->fd_out = open(node->rdr.file, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if (cmd->fd_out < 0)
	{
		redir_error(node);
		return (1);
	}
	return (0);
}

int	make_redirs(t_ast *node)
{
	t_n_type	type;
	int			i;

	i = -1;
	while (node->children && node->children[++i])
	{
		type = node->children[i]->type;
		if (type == NODE_REDIR_IN
			&& make_redir_in(node->children[i], &node->cmd))
			break ;
		else if (type == NODE_REDIR_OUT
			&& make_redir_out(node->children[i], &node->cmd))
			break ;
		else if (type == NODE_REDIR_APPEND
			&& make_redir_append(node->children[i], &node->cmd))
			break ;
		else if (type == NODE_HEREDOC)
			make_heredoc(node->children[i], &node->cmd);
	}
	return (check_redirs(node, &node->cmd));
}
